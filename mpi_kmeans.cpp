#include <omp.h>
#include "mpi_kmeans.h"
#include "util.h"

void mpi_kmeans(int numClusters,
                int numObjs,
                int numDims,
                int maxIter,
                double threshold,
                int *label,
                double **data,
                double **clusters,
                MPI_Comm   comm) {

    
    int     rank;
    int     total_numObjs;
    MPI_Comm_rank(comm, &rank);

    double **localClusters;
    int *localClusterSize;
    int *globalClusterSize;
    
    malloc2D(localClusters, numClusters, numDims, double);
    malloc1D(localClusterSize, numClusters, int);
    malloc1D(globalClusterSize, numClusters, int);

    for (int i = 0; i < numClusters; i++) {
        localClusterSize[i] = 0;
        for (int j = 0; j < numDims; j++) {
            localClusters[i][j] = 0;
        }
    }

    double localDelta, globalDelta;
    int iter = 0;

    MPI_Allreduce(&numObjs, &total_numObjs, 1, MPI_INT, MPI_SUM, comm);

    do {
        
        localDelta = 0;
        for (int i=0; i < numObjs; i++) {

            // Compute the cloest distance for each data point
            int curLabel = 0;
            double ans = compute_euclid(numDims, data[i], clusters[0]);
            for (int j = 1; j < numClusters; j++) {
                double dis = compute_euclid(numDims, data[i], clusters[j]);
                if (ans > dis) {
                    curLabel = j;
                    ans = dis;
                }
            }
            if (curLabel != label[i]) localDelta += 1;

            label[i] = curLabel;

            // Average the data points in the cluster
            for (int j=0; j < numDims; j++) {
                localClusters[curLabel][j] += data[i][j];
                // localClusters[curLabel][j] = (localClusters[curLabel][j] * localClusterSize[curLabel]  + data[i][j]) / (localClusterSize[curLabel] + 1);
            }
            localClusterSize[curLabel] += 1;

        }

        /* sum all data objects in newClusters */
        MPI_Allreduce(localClusters[0], clusters[0], numClusters*numDims,
                      MPI_DOUBLE, MPI_SUM, comm);
        MPI_Allreduce(localClusterSize, globalClusterSize, numClusters, MPI_INT,
                      MPI_SUM, comm);

        MPI_Allreduce(&localDelta, &globalDelta, 1, MPI_DOUBLE, MPI_SUM, comm);

        // Compute delta distance and update centeroids
        for (int i = 0; i < numClusters; i++) {
            
            for (int j = 0; j < numDims; j++) {
                if (globalClusterSize[i] > 0) {
                    clusters[i][j] /= globalClusterSize[i];
                } else {
                    clusters[i][j] = 0;
                }
                localClusters[i][j] = 0;
            }
            localClusterSize[i] = 0;
        }

        localDelta = globalDelta / total_numObjs;
        iter += 1;

    } while (localDelta > threshold && iter < maxIter);

    free1D(globalClusterSize);
    free1D(localClusterSize);
    free2D(localClusters);

}

void hybrid_kmeans(int numClusters,
                int numObjs,
                int numDims,
                int maxIter,
                double threshold,
                int *label,
                double **data,
                double **clusters,
                MPI_Comm   comm) {

    
    int     rank;
    int     total_numObjs;
    MPI_Comm_rank(comm, &rank);

    int nthreads;
    int *globalClusterSize;

    double **localClusters;
    int *localClusterSize;

    int **thread_clusterSize; /* [nthreads][numClusters] */
    double ***thread_clusters; /* [nthreads][numClusters][numCoords] */

    nthreads = omp_get_max_threads();
    malloc3D(thread_clusters, nthreads, numClusters, numDims, double);
    malloc2D(thread_clusterSize, nthreads, numClusters, int);

    malloc2D(localClusters, numClusters, numDims, double);
    malloc1D(localClusterSize, numClusters, int);
    malloc1D(globalClusterSize, numClusters, int);

    for (int i = 0; i < numClusters; i++) {
        localClusterSize[i] = 0;
        for (int j = 0; j < numDims; j++) {
            localClusters[i][j] = 0;
        }
    }

    for (int i = 0; i < nthreads; i++) {
        for (int j = 0; j < numClusters; j++) {
            thread_clusterSize[i][j] = 0;
            for (int k = 0; k < numDims; k++) {
                thread_clusters[i][j][k] = 0;
            }
            
        }
    }

    double localDelta, globalDelta;
    int iter = 0;

    MPI_Allreduce(&numObjs, &total_numObjs, 1, MPI_INT, MPI_SUM, comm);

    do {
        
        localDelta = 0;

        #pragma omp parallel \
                    shared(data,clusters,label,thread_clusters,thread_clusterSize)
        {
            int tid = omp_get_thread_num();
            #pragma omp for \
                            firstprivate(numObjs,numClusters,numDims) \
                            schedule(static) \
                            reduction(+:localDelta)
            for (int i=0; i < numObjs; i++) {

                // Compute the cloest distance for each data point
                int curLabel = 0;
                double ans = compute_euclid(numDims, data[i], clusters[0]);
                for (int j = 1; j < numClusters; j++) {
                    double dis = compute_euclid(numDims, data[i], clusters[j]);
                    if (ans > dis) {
                        curLabel = j;
                        ans = dis;
                    }
                }
                if (curLabel != label[i]) localDelta += 1;

                label[i] = curLabel;

                // Average the data points in the cluster
                for (int j=0; j < numDims; j++) {
                    thread_clusters[tid][curLabel][j] += data[i][j];
                }
                thread_clusterSize[tid][curLabel] += 1;
            }
        }

        // Aggregrate the results from threads
        for (int i = 0; i < numClusters; i++) {
            for (int j = 0; j < numDims; j++) {
                for (int tid = 0; tid < nthreads; tid++) {
                    localClusters[i][j] += thread_clusters[tid][i][j];
                    thread_clusters[tid][i][j] = 0;

                    localClusterSize[i] += thread_clusterSize[tid][i];
                    thread_clusterSize[tid][i] = 0;
                }
                
            }
        }

        /* sum all data objects in newClusters */
        MPI_Allreduce(localClusters[0], clusters[0], numClusters*numDims,
                      MPI_DOUBLE, MPI_SUM, comm);
        MPI_Allreduce(localClusterSize, globalClusterSize, numClusters, MPI_INT,
                      MPI_SUM, comm);

        MPI_Allreduce(&localDelta, &globalDelta, 1, MPI_DOUBLE, MPI_SUM, comm);

        // Compute delta distance and update centeroids
        for (int i = 0; i < numClusters; i++) {
            
            for (int j = 0; j < numDims; j++) {
                if (globalClusterSize[i] > 0) {
                    clusters[i][j] /= globalClusterSize[i];
                } else {
                    clusters[i][j] = 0;
                }
                localClusters[i][j] = 0;
            }
            localClusterSize[i] = 0;
        }

        localDelta = globalDelta / total_numObjs;
        iter += 1;

    } while (localDelta > threshold && iter < maxIter);

    free1D(globalClusterSize);
    free1D(localClusterSize);
    free2D(localClusters);

    free3D(thread_clusters);
    free2D(thread_clusterSize);

}
