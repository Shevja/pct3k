#include <stdio.h>
#include <malloc.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    int n = 1;//1KB, 1 MB
    FILE *fp = fopen("time.txt", "w");
    fprintf(fp,"Message size: %dB\n\n", n);
    fclose(fp);

    int rank, commsize;
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);

    char *sbuf = malloc(n * sizeof(char));
    char *rbuf = malloc(n * sizeof(char) * (commsize-1));
    // if (rank == 1)
    // {
    //     sbuf[0] = 'a';
    // }
    // else if (rank == 2)
    // {
    //     sbuf[0] = 'b';
    // }
    // else if (rank == 3)
    // {
    //     sbuf[0] = 'c';
    // }

    

    double time = MPI_Wtime();
    if (rank > 0)
    {
        MPI_Send(sbuf, n, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }
    else
    {
        MPI_Status stat;
        for (int i = 0; i < commsize-1; i++)
        {
            MPI_Recv(rbuf + i * n, n*(commsize-1), MPI_CHAR, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &stat); 
        }
    }
    time = MPI_Wtime() - time;

    // if (rank == 0)
    //     printf("process %d: %s\n", rank, rbuf);

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