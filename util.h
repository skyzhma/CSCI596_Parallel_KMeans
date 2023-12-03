#include <stdio.h>
#include <string>
#include <assert.h>

using namespace std;

#define malloc2D(name, xDim, yDim, type) do {               \
    name = (type **)malloc(xDim * sizeof(type *));          \
    assert(name != NULL);                                   \
    name[0] = (type *)malloc(xDim * yDim * sizeof(type));   \
    assert(name[0] != NULL);                                \
    for (size_t i = 1; i < xDim; i++)                       \
        name[i] = name[i-1] + yDim;                         \
} while (0)

#define free2D(name, xDim) do {               \
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

void seperate_data(string filePath, int numDivisions);

double compute_euclid(int dimension, double *coord1, double *coord2);
