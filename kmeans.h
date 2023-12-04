#include <stdio.h>


void sequential(int numCluster,
                int numObjs,
                int numDims,
                int maxIter,
                double threshold,
                int *label,
                double **data,
                double **centers);


void omp(int numCluster,
                int numObjs,
                int numDims,
                int maxIter,
                double threshold,
                int *label,
                double **data,
                double **centers);