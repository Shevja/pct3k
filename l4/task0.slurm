#!/bin/bash

#SBATCH --nodes=1 --ntasks-per-node=4
#SBATCH --job-name=mpitask

cd $SLURM_SUBMIT_DIR

mpiexec ./jacob 10000 10000