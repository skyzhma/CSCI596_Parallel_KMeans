#include<random>
#include <iostream>
#include <fstream>
#include <sstream>
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

void read_data(const std::string &filePath, float **data) {
    
    int numObjs, numDims;
    read_file_name(filePath, numObjs, numDims);

    malloc2D(data, numObjs, numDims, float);

    std::ifstream inputFile(filePath);
    if (inputFile.is_open()) {
        std::string line;
        int i = 0;
        while (std::getline(inputFile, line)) {
            
            std::istringstream iss(line);

            // Read three float numbers from each line
            int j = 0;
            float num;
            while (iss >> num) {
                data[i][j++] = num;
            }

            j = 0;
            i++;
        }

        inputFile.close();

        // Print the content of the 2D array
        for (int i = 0; i < numObjs; i++) {
            for (int j = 0; j < numDims; j++) {
                std::cout << data[i][j] << " ";
            }
            std::cout << std::endl;
        }

        free2D(data, numObjs);
        
    } else {
        std::cerr << "Error opening the file for reading." << std::endl;
    }


}

void generate_file_name(std::string &filePath, const int &numObjs, const int &numDims) {
    filePath = filePath + "-" + std::to_string(numObjs) + "-" + std::to_string(numDims) + ".txt";
}

void read_file_name(const std::string &fileName, int &numObjs, int &numDims) {
    
    size_t firstDashPos = fileName.find('-');
    size_t secondDashPos = fileName.find('-', firstDashPos + 1);
    assert(firstDashPos != std::string::npos);
    assert(secondDashPos != std::string::npos);

    std::string numObjsStr = fileName.substr(firstDashPos + 1, secondDashPos - firstDashPos - 1);
    std::string numDimsStr = fileName.substr(secondDashPos + 1, fileName.size() - secondDashPos - 5); // "-.txt" has 5 characters

    std::istringstream(numObjsStr) >> numObjs;
    std::istringstream(numDimsStr) >> numDims;

}

// void seperate_data(const std::string &filePath, int numDivisions);

float compute_euclid(int numDimension, float *coord1, float *coord2) {

    float dis = .0f;

    for (int i = 0; i < numDimension; i++) {
        dis += (coord1[i] - coord2[i]) * (coord1[i] - coord2[i]);
    }

    return dis;

}
