#include <stdio.h>
#include <malloc.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    int n = 1;//1B, 1KB, 1MB
    int rank, commsize;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int right = (rank + 1) % commsize;
    int left = (rank - 1 + commsize) % commsize;
    double time;

    char *sbuf;
    char *rbuf;

    if (rank == 0)
        printf("Message size: %dB\n", n);
    sbuf = malloc(n * sizeof(char));
    sbuf[0] = 'a';
    rbuf = malloc(n * sizeof(char));

    time = MPI_Wtime();
    for (int i = 0; i < commsize - 1; i++)
    {
        MPI_Status stat[2];
        MPI_Request reqs[2];
        MPI_Isend(sbuf, n, MPI_CHAR, right, 0, MPI_COMM_WORLD, &reqs[0]);
        MPI_Irecv(rbuf, n, MPI_CHAR, left, 0, MPI_COMM_WORLD, &reqs[1]);

        MPI_Waitall(2, reqs, stat);

        char *p = sbuf;
        sbuf = rbuf;
        rbuf = p;
    }
    time = MPI_Wtime() - time;

    printf("process %d: %f\n", rank, time);

    MPI_Finalize();
    return 0;
}