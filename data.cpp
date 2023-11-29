#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include "util.h"

int main(int argc, char **argv) {

    int         opt;
    int         numDims;              // number of vector dimensions
    int         numObjs;              // number of data points
    std::string filePath;             // path of the data
    bool        del;                  // to delete the data file or not

    del = true;
    // Parse Args
    while ((opt = getopt(argc, argv, "d:o:p:g:")) != -1) {
        switch (opt) {
            case 'd':
                numDims = atoi(optarg);
                break;
            case 'o':
                numObjs=atoi(optarg);
                break;
            case 'p':
                filePath = optarg;
                break;
            case 'g':
                if (atoi(optarg)) {
                    del = false;
                }
        }
    }

    if (del) {
        delete_data(filePath, numObjs, numDims);
    } else {
        generate_data(filePath, numObjs, numDims);
    }

    return 0;

}