#include<random>
#include <iostream>
#include <fstream>
#include <sstream>
#include "util.h"


void generate_data(string filePath, int numObjs, int numDims) {

    // Use a random device to seed the random number generator
    random_device rd;
    mt19937 gen(rd());

    // Generate random doubleing-point numbers in the range [0.0, 1.0]
    uniform_real_distribution<double> doubleDistribution(-20, 20);

    // Open a file for writing
    string fileName = filePath;
    generate_file_name(fileName, numObjs, numDims);
    ofstream outputFile(fileName);
    if (!outputFile.is_open()) {
        cerr << "Error opening the file for writing." << endl;
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

    cout << "Random numbers have been written to " << fileName << "\n";

}

void delete_data(string filePath, int numObjs, int numDims) {

    string fileName = filePath;
    generate_file_name(fileName, numObjs, numDims);

    if (remove(fileName.c_str()) != 0) {
        cerr << "Error deleting file: " << fileName << endl;
    } else {
        cout << "File deleted successfully: " << fileName << endl;
    }   
}

void read_data(string filePath, double **data) {
    
    ifstream inputFile(filePath);
    if (inputFile.is_open()) {
        string line;
        int i = 0;
        while (getline(inputFile, line)) {

            istringstream iss(line);

            // Read double numbers from each line
            int j = 0;
            double num;
            while (iss >> num) {
                data[i][j++] = num;
            }

            i++;
        }

        inputFile.close();

    } else {
        cerr << "Error opening the file for reading." << endl;
    }


}

void generate_file_name(string &filePath, int numObjs, int numDims) {
    filePath = filePath + "-" + to_string(numObjs) + "-" + to_string(numDims) + ".txt";
}

void read_file_name(string &fileName, int &numObjs, int &numDims) {
    
    size_t firstDashPos = fileName.find('-');
    size_t secondDashPos = fileName.find('-', firstDashPos + 1);
    assert(firstDashPos != string::npos);
    assert(secondDashPos != string::npos);

    string numObjsStr = fileName.substr(firstDashPos + 1, secondDashPos - firstDashPos - 1);
    string numDimsStr = fileName.substr(secondDashPos + 1, fileName.size() - secondDashPos - 5); // "-.txt" has 5 characters

    istringstream(numObjsStr) >> numObjs;
    istringstream(numDimsStr) >> numDims;

}

// void seperate_data(string &filePath, int numDivisions);

double compute_euclid(int numDimension, double *coord1, double *coord2) {

    double dis = .0f;

    for (int i = 0; i < numDimension; i++) {
        dis += (coord1[i] - coord2[i]) * (coord1[i] - coord2[i]);
    }

    return dis;

}
