#include <stdio.h>
#include <string>
#include <assert.h>

#define malloc2D(name, xDim, yDim, type) do {               \
    name = (type **)malloc(xDim * sizeof(type *));          \
    assert(name != NULL);                                   \
    name[0] = (type *)malloc(xDim * yDim * sizeof(type));   \
    assert(name[0] != NULL);                                \
    for (size_t i = 1; i < xDim; i++)                       \
        name[i] = name[i-1] + yDim;                         \
} while (0)

void generate_data(const std::string &filePath, const int &numObjs, const int &numDims);

void delete_data(const std::string &filePath, const int &numObjs, const int &numDims);

void generate_file_name(std::string &filePath, const int &numObjs, const int &numDims);

void seperate_data(const std::string &filePath, int numDivisions);

float compute_euclid(int dimension, float *coord1, float *coord2);