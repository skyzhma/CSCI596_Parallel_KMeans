#include<random>
#include <iostream>
#include <fstream>
#include "util.h"


void generate_data(const std::string &filePath, const int &numObjs, const int &numDims) {

    // Use a random device to seed the random number generator
    std::random_device rd;
    std::mt19937 gen(rd());

    // Generate random floating-point numbers in the range [0.0, 1.0]
    std::uniform_real_distribution<double> doubleDistribution(0.0, 1.0);
    std::uniform_real_distribution<float> floatDistribution(-10, 10);
    std::uniform_int_distribution intDistribution(-10, 10);

    // Open a file for writing
    std::string fileName = filePath;
    generate_file_name(fileName, numObjs, numDims);
    std::ofstream outputFile(fileName);
    if (!outputFile.is_open()) {
        std::cerr << "Error opening the file for writing." << std::endl;
    }

    // Store the generated numbers in a txt file
    for (int i = 0; i < numObjs; ++i) {
        for (int j = 0; j < numDims; ++j) {
            outputFile << doubleDistribution(gen);
            outputFile << (j == numDims - 1 ? "\n" : " ");
        }
    }
    
    // Close the file
    outputFile.close();

    std::cout << "Random numbers have been written to " << fileName << "\n";

}

void delete_data(const std::string &filePath, const int &numObjs, const int &numDims) {

    std::string fileName = filePath;
    generate_file_name(fileName, numObjs, numDims);

    if (std::remove(fileName.c_str()) != 0) {
        std::cerr << "Error deleting file: " << fileName << std::endl;
    } else {
        std::cout << "File deleted successfully: " << fileName << std::endl;
    }   
}

void generate_file_name(std::string &filePath, const int &numObjs, const int &numDims) {
        
    filePath = filePath + std::to_string(numObjs) + "-" + std::to_string(numDims) + ".txt";

}
// void seperate_data(const std::string &filePath, int numDivisions);

float compute_euclid(int numDimension, float *coord1, float *coord2) {

    float dis = .0f;

    for (int i = 0; i < numDimension; i++) {
        dis += (coord1[i] - coord2[i]) * (coord1[i] - coord2[i]);
    }

    return dis;

}
