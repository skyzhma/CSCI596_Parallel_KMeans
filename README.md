# CSCI596-Final-Project

This repository is mainly about parallelizing KMeans algorithm.

# Introduction to the KMeans Algorithm

KMeans is a popular clustering algorithm used in unsupervised learning tasks. Suppose that a dataset has N data points and the major goal of the KMeans algorithm is to partition it into K clusters where K is a hyper parameter. 

![](./Kmeans.gif)

## Steps
1. Randomly initialize K cluster centriods
2. For each data point, compute its Euclidean distance to the centriods and assign the data point to the cluster whose centroid is the cloest
3. Recalculate the centroid of each cluster by averaging all the data points in that cluster
4. Repeat step 2 and 3 until the algorithm converages

# Parallelization
In step 2, we need to devide the cloest centroid for each data points. Since the centroid are determined, we can create multiple threads and each thread will compute the distance for some data points at the same time. 

* Small Scale : We decide to use OpenMP to implement the parallelization. 
* Large Scale : We decide to use MPI + OpenMP to parallelize the algorithm. The master will store the centroids and pass it to multiple workers. Each worker will handle a part of the dataset, compute the distance for each data point. Master and works will share the information of the centroids.

We will randomly generate the datasets for training

# Todo
## Code
* Sequential KMeans Implentation
* OpenMP Implementation

### MPI Implementation
Before compiling the program, run the following command in your CARC terminal
```
export LD_PRELOAD=/spack/apps/gcc/8.3.0/lib64/libstdc++.so.6
```

##  Results
* Compare the performance of Sequential implementation with the OpenMP implementation with various size of dataset as input.
* Compare the performance of OpenMP implementations with different number of thread settings.
* Compare the performance of MPI+OpenMP implementation with different number of node settings and various size of datasets.

