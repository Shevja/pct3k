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
    char *rbuf = malloc(n * sizeof(char));

    double time = MPI_Wtime();
    for (int i = 0; i < commsize-1; i++)
    {
        MPI_Request reqs[commsize-2];
        MPI_Status stat[commsize-2];

        for (int i = 0, j = 0; i < commsize-1; i++, j++)
        {
            if (rank != i)
            {
                MPI_Irecv(rbuf, n, MPI_CHAR, i, 0, MPI_COMM_WORLD, &reqs[j]);
            }
            else
            {
                j--;
            }
        }
        for (int i = 0, j = 0; i < commsize-1; i++, j++)
        {
            if (rank != i)
            {
                MPI_Isend(sbuf, n, MPI_CHAR, i, 0, MPI_COMM_WORLD, &reqs[j]);
            }
            else
            {
                j--;
            }
        }

        MPI_Waitall(2, reqs, stat);
    }
    time = MPI_Wtime() - time;

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