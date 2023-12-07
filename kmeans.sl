#!/bin/bash
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=24
#SBATCH --output=seq_omp.out
#SBATCH --mem=0
#SBATCH -A anakano_429
#SBATCH --oversubscribe

export LD_PRELOAD=/spack/apps/gcc/8.3.0/lib64/libstdc++.so.6

fileName="data-10000000-32.txt"

./main -s $fileName

./main -s $fileName -t 2

./main -s $fileName -t 4

./main -s $fileName -t 8

./main -s $fileName -t 16

./main -s $fileName -t 24

