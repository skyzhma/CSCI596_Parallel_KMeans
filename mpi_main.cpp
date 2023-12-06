#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <omp.h>
#include "mpi_kmeans.h"
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
    double  **clusters;            // coord of new clusters
    string  filePath;             // fileName of dataset


    // Default Values
    numClusters = 4;
    numThreads = 0;
    numObjs = 1000;
    numDims = 64;
    filePath = "data-1000-64.txt";
    maxIter = 500;
    threshold = .001;

    // Initialize MPI
    int     rank, nproc;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);

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

    // Read and distribute data
    int divd, rem;

    divd = (numObjs) / nproc;
    rem  = (numObjs) % nproc;
    
    // Initialize data
    malloc1D(label, numObjs, int);
    malloc2D(clusters, numClusters, numDims, double);
    for (int i=0; i < numObjs; i++) label[i] = -1;

    if (rank == 0) {

        // Read Dataset
        malloc2D(data, numObjs, numDims, double);
        read_data(filePath, data);

        // Initialize Centroids
        for (int i = 0; i < numClusters; i++) {
            for (int j = 0; j < numDims; j++) {
                clusters[i][j] = data[i][j];
            }
        }

        int index = (rem > 0) ? divd+1 : divd;
        numObjs = index;

        // distribute objects[] to other processes
        for (int i=1; i<nproc; i++) {
            int msg_size = (i < rem) ? (divd+1) : divd;
            MPI_Send(data[index], msg_size*numDims, MPI_DOUBLE, i, i, MPI_COMM_WORLD);
            MPI_Send(clusters[0], numClusters * numDims, MPI_DOUBLE, i, i, MPI_COMM_WORLD);
            index += msg_size;

        }

        // Reallocate data
        data[0] = (double *)realloc(data[0], numObjs * numDims * sizeof(double));
        assert(data[0] != NULL);
        data    = (double **)realloc(data, numObjs * sizeof(double *));

    } else {

        // Allocate Memory for subprocesses
        numObjs = (rank < rem) ? divd+1 : divd;
        
        malloc2D(data, numObjs, numDims, double);
        MPI_Recv(data[0], numObjs * numDims, MPI_DOUBLE, 0, rank, MPI_COMM_WORLD, &status);
        MPI_Recv(clusters[0], numClusters * numDims, MPI_DOUBLE, 0, rank, MPI_COMM_WORLD, &status);

    }

    // Set timing
    double stime = MPI_Wtime();

    // Implementation of Algorithm
    if (numThreads > 0) {
        omp_set_num_threads(numThreads);
        hybrid_kmeans(numClusters,
        numObjs,
        numDims,
        maxIter,
        threshold,
        label,
        data,
        clusters,
        MPI_COMM_WORLD
        ); 

    } else {
        mpi_kmeans(numClusters,
        numObjs,
        numDims,
        maxIter,
        threshold,
        label,
        data,
        clusters,
        MPI_COMM_WORLD
        ); 
    }


    MPI_Barrier(MPI_COMM_WORLD);
    // Set barrier
    if (rank == 0) {

        for (int i = 0; i < numClusters; i++) {
            std::cout << i << " cetroid ";
            for (int j = 0; j < numDims; j ++) {
            std::cout << clusters[i][j] << " ";
            }
            std::cout << endl;
        }
        // Output
        double etime = MPI_Wtime();
        cout << "Total time :" << etime - stime << endl;
    }

    // free memory
    free1D(label);
    free2D(data);
    free2D(clusters);

    MPI_Finalize();

    return 0;
}