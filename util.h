#include <stdio.h>
#include <string>
#include <assert.h>

using namespace std;

#define malloc3D(name, xDim, yDim, zDim, type) do {                \
    name = (type ***)malloc(xDim * sizeof(type **));               \
    assert(name != NULL);                                          \
    name[0] = (type **)malloc(xDim * yDim * sizeof(type *));      \
    assert(name[0] != NULL);                                       \
    name[0][0] = (type *)malloc(xDim * yDim * zDim * sizeof(type)); \
    assert(name[0][0] != NULL);                                    \
    for (size_t i = 0; i < xDim; i++) {                            \
        name[i] = name[0] + i * yDim;                              \
        for (size_t j = 0; j < yDim; j++) {                       \
            name[i][j] = name[0][0] + (i * yDim + j) * zDim;      \
        }                                                          \
    }                                                              \
} while (0)

#define free3D(name) do {      \
    free(name[0][0]);           \
    free(name[0]);               \
    free(name);                  \
} while (0)


#define malloc2D(name, xDim, yDim, type) do {               \
    name = (type **)malloc(xDim * sizeof(type *));          \
    assert(name != NULL);                                   \
    name[0] = (type *)malloc(xDim * yDim * sizeof(type));   \
    assert(name[0] != NULL);                                \
    for (size_t i = 1; i < xDim; i++)                       \
        name[i] = name[i-1] + yDim;                         \
} while (0)

#define free2D(name) do {               \
    free(name[0]);                            \
    free(name);                               \
} while (0)


#define malloc1D(name, xDim, type) do {              \
    name = (type *)malloc(xDim * sizeof(type));      \
    assert(name != NULL);                            \
} while (0)

#define free1D(name) do {           \
    free(name);                     \
} while (0)

void generate_data(string filePath, int numObjs, int numDims);

void delete_data(string filePath, int numObjs, int numDims);

void read_data(string filePath, double **data);

void generate_file_name(string &filePath, int numObjs, int numDims);

void read_file_name(string &fileName, int &numObjs, int &numDims);

void print_centroids(double **centroids, int numClusters, int numDims);

double compute_euclid(int dimension, double *coord1, double *coord2);
