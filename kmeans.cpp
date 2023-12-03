#include <stdio.h>
#include <iostream>
#include <math.h>
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

    double **localCenters;
    int *localCenterSize;
    malloc2D(localCenters, numClusters, numDims, double);
    malloc1D(localCenterSize, numClusters, int);

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
                localCenters[curLabel][j] += data[i][j];
                // localCenters[curLabel][j] = (localCenters[curLabel][j] * localCenterSize[curLabel]  + data[i][j]) / (localCenterSize[curLabel] + 1);
            }
            localCenterSize[curLabel] += 1;

        }

        // Compute delta distance and update centeroids
        for (int i = 0; i < numClusters; i++) {
            
            // delta += compute_euclid(numDims, centers[i], localCenters[i]); 

            for (int j = 0; j < numDims; j++) {
                localCenters[i][j] /= localCenterSize[i];
                centers[i][j] = localCenters[i][j];
                localCenters[i][j] = 0;
            }
            localCenterSize[i] = 0;
        }

        delta /= numObjs;
        iter += 1;

    } while (delta > threshold && iter < maxIter);

    free1D(localCenterSize);
    free2D(localCenters, numClusters);

}
