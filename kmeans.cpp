#include <stdio.h>
#include <iostream>
#include <math.h>
#include <omp.h>
#include "kmeans.h"
#include "util.h"

void sequential(int numClusters,
                int numObjs,
                int numDims,
                int maxIter,
                double threshold,
                int *label,
                double **data,
                double **centers) {

    double **localClusters;
    int *localClusterSize;
    malloc2D(localClusters, numClusters, numDims, double);
    malloc1D(localClusterSize, numClusters, int);

    for (int i = 0; i < numClusters; i++) {
        localClusterSize[i] = 0;
        for (int j = 0; j < numDims; j++) {
            localClusters[i][j] = 0;
        }
    }

    double delta;
    int iter = 0;

    do {
        
        delta = 0;
        for (int i=0; i < numObjs; i++) {

            // Compute the cloest distance for each data point
            int curLabel = 0;
            double ans = compute_euclid(numDims, data[i], centers[0]);
            for (int j = 1; j < numClusters; j++) {
                double dis = compute_euclid(numDims, data[i], centers[j]);
                if (ans > dis) {
                    curLabel = j;
                    ans = dis;
                }
            }
            if (curLabel != label[i]) delta += 1;

            label[i] = curLabel;

            // Average the data points in the cluster
            for (int j=0; j < numDims; j++) {
                localClusters[curLabel][j] += data[i][j];
                // localClusters[curLabel][j] = (localClusters[curLabel][j] * localClusterSize[curLabel]  + data[i][j]) / (localClusterSize[curLabel] + 1);
            }
            localClusterSize[curLabel] += 1;

        }

        // Compute delta distance and update centeroids
        for (int i = 0; i < numClusters; i++) {
            
            // delta += compute_euclid(numDims, centers[i], localClusters[i]); 

            for (int j = 0; j < numDims; j++) {
                localClusters[i][j] /= localClusterSize[i];
                centers[i][j] = localClusters[i][j];
                localClusters[i][j] = 0;
            }
            localClusterSize[i] = 0;
        }

        delta /= numObjs;
        iter += 1;

    } while (delta > threshold && iter < maxIter);

    free1D(localClusterSize);
    free2D(localClusters);

}


void omp(int numClusters,
                int numObjs,
                int numDims,
                int maxIter,
                double threshold,
                int *label,
                double **data,
                double **centers) {

    // double **localClusters;
    // int *localClusterSize;
    // malloc2D(localClusters, numClusters, numDims, double);
    // malloc1D(localClusterSize, numClusters, int);

    int      nthreads;             /* no. threads */
    int     *globalClusterSize;
    int    **localClusterSize; /* [nthreads][numClusters] */
    double ***localClusters;    /* [nthreads][numClusters][numCoords] */

    nthreads = omp_get_max_threads();
    
    malloc3D(localClusters, nthreads, numClusters, numDims, double);
    malloc2D(localClusterSize, nthreads, numClusters, int);
    malloc1D(globalClusterSize, numClusters, int);

    for (int i = 0; i < nthreads; i++) {
        for (int j = 0; j < numClusters; j++) {
            localClusterSize[i][j] = 0;
            globalClusterSize[j] = 0;
            for (int k = 0; k < numDims; k++) {
                localClusters[i][j][k] = 0;
            }
            
        }
    }

    double delta;
    int iter = 0;

    do {
        
        delta = 0;

        #pragma omp parallel \
                    shared(data,centers,label,localClusters,localClusterSize)
        {
            int tid = omp_get_thread_num();
            #pragma omp for \
                            firstprivate(numObjs,numClusters,numDims) \
                            schedule(static) \
                            reduction(+:delta)
            for (int i=0; i < numObjs; i++) {

                // Compute the cloest distance for each data point
                int curLabel = 0;
                double ans = compute_euclid(numDims, data[i], centers[0]);
                for (int j = 1; j < numClusters; j++) {
                    double dis = compute_euclid(numDims, data[i], centers[j]);
                    if (ans > dis) {
                        curLabel = j;
                        ans = dis;
                    }
                }
                if (curLabel != label[i]) delta += 1;

                label[i] = curLabel;

                // Average the data points in the cluster
                for (int j=0; j < numDims; j++) {
                    localClusters[tid][curLabel][j] += data[i][j];
                    // localClusters[curLabel][j] = (localClusters[curLabel][j] * localClusterSize[curLabel]  + data[i][j]) / (localClusterSize[curLabel] + 1);
                }
                localClusterSize[tid][curLabel] += 1;

            }

        }

        // Compute delta distance and update centeroids
        for (int i = 0; i < numClusters; i++) {

            for (int j = 0; j < numDims; j++) {
                double vector_sum = 0;
                for (int tid = 0; tid < nthreads; tid++) {
                    vector_sum += localClusters[tid][i][j];
                    localClusters[tid][i][j] = 0;

                    globalClusterSize[i] += localClusterSize[tid][i];
                    localClusterSize[tid][i] = 0;
                }
                
                if (globalClusterSize[i] > 0) {
                    centers[i][j] = vector_sum / globalClusterSize[i];   
                } else {
                    centers[i][j] = 0;   
                }
            }

            globalClusterSize[i] = 0;
        }

        delta /= numObjs;
        iter += 1;
    } while (delta > threshold && iter < maxIter);

    free1D(globalClusterSize);
    free2D(localClusterSize);
    free3D(localClusters);

}
