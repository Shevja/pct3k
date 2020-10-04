#include <stdio.h>
#include <malloc.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    int n = 1;//1B, 1KB, 1MB
    FILE *fp = fopen("time.txt", "a+");
    fprintf(fp,"\nMessage size: %dB\n", n);
    fclose(fp);
    
    int rank, commsize;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status stat;
    
    char *m = malloc(n * sizeof(char));
    // if (rank == 0)
    //     m[0] = 'v';
    char *m1 = malloc(n * sizeof(char));

    int right = (rank + 1) % commsize;
    int left = (rank - 1 + commsize) % commsize;
    double time;
    
    time = MPI_Wtime();
    if (rank == 0)
    {
        MPI_Send(m, n, MPI_CHAR, right, 0, MPI_COMM_WORLD);
        MPI_Recv(m1, n, MPI_CHAR, left, 0, MPI_COMM_WORLD, &stat);
    }
    else
    {
        MPI_Recv(m, n, MPI_CHAR, left, 0, MPI_COMM_WORLD, &stat);
        MPI_Send(m, n, MPI_CHAR, right, 0, MPI_COMM_WORLD);
    }
    time = MPI_Wtime() - time;

    // printf("process %d: %c\n", rank, m[0]);

    fp = fopen("time.txt", "a+");
    if (fp == NULL)
    {
        printf("Can't open file\n");
        return -1;
    }
    fprintf(fp, "process %d: %f\n", rank, time);
    fclose(fp);

    MPI_Finalize();
    return 0;
}