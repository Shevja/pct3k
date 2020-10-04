#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv){
    int rank, commsize;
    
    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    printf("Hello, I am process %d of %d\n", rank, commsize);
    
    MPI_Finalize();
    
    return 0;
}