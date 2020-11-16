#include <stdio.h>
#include <malloc.h>
#include <mpi.h>

int mc(char *to, char *from, int n)
{
    for (int i = 0; i < n; i++)
    {
        to[i] = from[i];
    }

    return 0;
}

int main(int argc, char **argv)
{
    int n = 1; //строго 1KB
    FILE *fp = fopen("time.txt", "w");
    fprintf(fp, "Message size: %dB\n\n", n);
    fclose(fp);

    int rank, commsize;
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);

    char *sbuf = malloc(n * sizeof(char));
    char *rbuf = malloc(n * sizeof(char) * (commsize - 1));

    double time = MPI_Wtime();
    MPI_Request reqs[(commsize - 1) * 2];
    MPI_Status stat[(commsize - 1) * 2];

    int j = 0;
    for (int i = 0; i < commsize; i++)
    {
        if (rank != i)
        {
            MPI_Irecv(rbuf + i * n, n, MPI_CHAR, i, 0, MPI_COMM_WORLD, &reqs[j++]);
            MPI_Isend(sbuf, n, MPI_CHAR, i, 0, MPI_COMM_WORLD, &reqs[j++]);
        }
        else
        {
            mc(rbuf + i * n, sbuf, n);
        }
    }

    MPI_Waitall(j, reqs, stat);

    time = MPI_Wtime() - time;

    // printf("process %d: %s\n", rank, rbuf);

    printf("process %d: %f\n", rank, time);

    MPI_Finalize();
    return 0;
}