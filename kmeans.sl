#!/bin/bash
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=16
#SBATCH --output=result.out
#SBATCH --mem=0
#SBATCH -A anakano_429
#SBATCH --oversubscribe

export LD_PRELOAD=/spack/apps/gcc/8.3.0/lib64/libstdc++.so.6

./data -o 100000 -d 128

./main -s data-100000-128.txt

./main -s data-100000-128.txt -t 2

./main -s data-100000-128.txt -t 4

./main -s data-100000-128.txt -t 8

mpirun -n 2 ./mpi_main -s data-100000-128.txt

mpirun -n 2 ./mpi_main -s data-100000-128.txt -t 2
