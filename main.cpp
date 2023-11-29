#include<stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "util.h"


int main(int argc, char **argv) {
    
    int     opt;
    int     numClusters;          // number of clusters
    int     numNodes;             // number of nodes
    int     numThreads;           // number of threads
    int     numDims;              // number of vector dimensions
    int     numObjs;              // number of data points 

    // Parse Args
    while ((opt = getopt(argc, argv, "k:n:t:d:o:")) != -1) {
        switch (opt) {
            case 'k':
                numClusters = atoi(optarg);
                break;
            case 'n':
                numNodes = atoi(optarg);
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
        }
    }

    // Randomly Generate Data

    // Separate Data

    // Initialize Centroids

    // Set timing 

    // Iteration

    // Output

    return 0;
}