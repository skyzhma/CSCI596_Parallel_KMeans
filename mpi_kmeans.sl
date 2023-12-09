#!/bin/bash
#SBATCH --nodes=8
#SBATCH --ntasks-per-node=2
#SBATCH --cpus-per-task=8
#SBATCH --output=mpi333.out
#SBATCH --mem=0
#SBATCH -A anakano_429
#SBATCH --oversubscribe

export LD_PRELOAD=/spack/apps/gcc/8.3.0/lib64/libstdc++.so.6

fileName="data-1000000-32.txt"


for k in 4 8 16
do
	for n in 2 4 8
	do
		mpirun -bind-to none -n $n ./mpi_main -s $fileName -k $k
		for t in 2 4 6 8 
		do
			mpirun -bind-to none -n $n ./mpi_main -s $fileName -k $k -t $t
		done
	done
done




fileName="data-10000000-32.txt"


for k in 4 8 16
do
        for n in 2 4 8
        do
                mpirun -bind-to none -n $n ./mpi_main -s $fileName -k $k
                for t in 2 4 6 8
                do
                        mpirun -bind-to none -n $n ./mpi_main -s $fileName -k $k -t $t
                done
        done
done
