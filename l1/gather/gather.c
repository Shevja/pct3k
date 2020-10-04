#include <stdio.h>
#include <malloc.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    int n = 1;
    FILE *fp = fopen("time.txt", "w");
    fprintf(fp,"Message size: %dB\n\n", n);
    fclose(fp);

    int rank, commsize;
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);

    char *sbuf = malloc(n * sizeof(char));
    char *rbuf = malloc(n * sizeof(char)*3);//поменять расширение памяти на приемлемое

    double time = MPI_Wtime();
    if (rank > 0)
    {
        MPI_Send(sbuf, n, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }
    else
    {
        MPI_Status stat;
        for (int i = 1; i < commsize; i++)
        {
            MPI_Recv(rbuf, n*3, MPI_CHAR, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &stat);//то же самое, n * 3? 
        }
    }
    time = MPI_Wtime() - time;

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