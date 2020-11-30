#!/bin/bash

#SBATCH --nodes=1 --ntasks-per-node=1
#SBATCH --job-name=mpitask

cd $SLURM_SUBMIT_DIR

mpiexec ./runge