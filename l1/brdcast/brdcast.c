#include <stdio.h>  
#include <malloc.h> 
#include <mpi.h>

int main(int argc, char **argv)
{
    int n = 1;//1KB, 1MB
    FILE *fp = fopen("time.txt", "w");
    fprintf(fp,"Message size: %dB\n\n", n);
    fclose(fp);

    int rank, commsize;
    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    char *sbuf = malloc(n * sizeof(char));
    // if (rank == 0)
    //     sbuf[0] = 'v';
    char *rbuf = malloc(n * sizeof(char));

    double time = MPI_Wtime();
    if (rank == 0)
    {
        for (int i = 1; i < commsize; i++)
        {
            MPI_Send(sbuf, n, MPI_CHAR, i, 0, MPI_COMM_WORLD);
        }
    }
    else
    {
        MPI_Status stat;
        MPI_Recv(rbuf, n, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &stat);
    }
    time = MPI_Wtime() - time;

    // printf("process %d: %c\n", rank, rbuf[0]);
    
    fp = fopen("time.txt", "a");
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