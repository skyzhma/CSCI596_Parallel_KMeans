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
