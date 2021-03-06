#include <stdio.h>
#include <malloc.h>
#include <mpi.h>

int mc(char *to, char *from, int n)
{
    for(int i = 0; i < n; i++)
    {
       to[i] = from[i];
       printf("%c", to[i]);
    }

    return 0;
}

int main(int argc, char **argv)
{
    int n = 1024;//1KB, 1 MB

    int rank, commsize;
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);

    char *sbuf = malloc(n * sizeof(char));
    char *rbuf = malloc(n * sizeof(char) * (commsize));

    double time = MPI_Wtime();
    if (rank > 0)
    {
        MPI_Send(sbuf, n, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }
    else
    {
        MPI_Status stat;
        for (int i = 0; i < commsize; i++)
        {
            if (i == 0)
            {
                mc(rbuf, sbuf, n);
            }
            else
            {
                MPI_Recv(rbuf + i * n, n, MPI_CHAR, i, 0, MPI_COMM_WORLD, &stat); 
            }
        }
    }
    time = MPI_Wtime() - time;

    if (rank == 0)
        printf("process %d: %s\n", rank, rbuf);
    printf("process %d: %f\n", rank, time);

    MPI_Finalize();

    return 0;
}