#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <omp.h>
#include "kmeans.h"
#include "util.h"

int main(int argc, char **argv) {
    
    int     opt;
    int     numClusters;          // number of clusters
    int     numThreads;           // number of threads
    int     numObjs;              // number of data points 
    int     numDims;              // number of vector dimensions
    int     maxIter;              // number of maximum iterations
    double  threshold;            // iteration threshold;
    int     *label;               // store the label of each data point
    double  **data;               // store the datasets
    double  **centers;            // coord of new centers
    string  filePath;             // fileName of dataset

    // Default Values
    numClusters = 4;
    numThreads = 0;
    numObjs = 10000;
    numDims = 64;
    filePath = "data-10000-64.txt";
    maxIter = 500;
    threshold = .001;

    // Parse Args
    while ((opt = getopt(argc, argv, "k:n:t:d:o:s:")) != -1) {
        switch (opt) {
            case 'k':
                numClusters = atoi(optarg);
                break;
            case 't':
                numThreads = atoi(optarg);
                break;
            case 'd':
                numDims = atoi(optarg);
                break;
            case 'o':
                numObjs=atoi(optarg);
                break;
            case 's':
                filePath = optarg;
                break;
        }
    }

    // Update numObjs and numDims based on the dataset
    read_file_name(filePath, numObjs, numDims);

    // Initialize data
    malloc1D(label, numObjs, int);
    malloc2D(data, numObjs, numDims, double);
    malloc2D(centers, numClusters, numDims, double);

    for (int i=0; i < numObjs; i++) label[i] = -1;

    // Read Data
    read_data(filePath, data);

    // Initialize Centroids
    for (int i = 0; i < numClusters; i++) {
        for (int j = 0; j < numDims; j++) {
            centers[i][j] = data[i][j];
        }
    }

    // Set timing
    double stime = omp_get_wtime();

    // Implementation of Algorithm

    if (numThreads > 0){
        omp_set_num_threads(numThreads);
        omp(numClusters,
                    numObjs,
                    numDims,
                    maxIter,
                    threshold,
                    label,
                    data,
                    centers
                    );
    } else {
        sequential(numClusters,
            numObjs,
            numDims,
            maxIter,
            threshold,
            label,
            data,
            centers
            );   
    }
        
    for (int i = 0; i < numClusters; i++) {
        std::cout << i << " cetroid ";
        for (int j = 0; j < numDims; j ++) {
           std::cout << centers[i][j] << " ";
        }
        std::cout << endl;
    }

    // Output
    double etime = omp_get_wtime();
    cout << "Total time :" << etime - stime << endl;

    // free memory
    free1D(label);
    free2D(data);
    free2D(centers);

    return 0;
}