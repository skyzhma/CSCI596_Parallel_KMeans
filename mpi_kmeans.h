#include <mpi.h>

void mpi_kmeans(int numCluster,
                int numObjs,
                int numDims,
                int maxIter,
                double threshold,
                int *label,
                double **data,
                double **clusters,
                MPI_Comm   comm);

void hybrid_kmeans(int numClusters,
                int numObjs,
                int numDims,
                int maxIter,
                double threshold,
                int *label,
                double **data,
                double **clusters,
                MPI_Comm   comm);