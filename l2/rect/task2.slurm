#!/bin/bash

#SBATCH --nodes=2 --ntasks-per-node=8
#SBATCH --job-name=mpitask

cd $SLURM_SUBMIT_DIR

mpiexec ./rect