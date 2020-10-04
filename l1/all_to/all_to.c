#include <stdio.h>
#include <malloc.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    int n = 1024; //строго 1KB
    FILE *fp = fopen("time.txt", "w");
    fprintf(fp,"Message size: %dB\n\n", n);
    fclose(fp);

    int rank, commsize;
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);

    char *sbuf = malloc(n * sizeof(char));
    char *rbuf = malloc(n * sizeof(char) * commsize-1);
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
    // else
    // {
    //     sbuf[0] = 'e';
    // }
    

    double time = MPI_Wtime();
    //for (int i = 0; i < commsize; i++)
    //{
        MPI_Request reqs[(commsize-1)*2];
        MPI_Status stat[(commsize-1)*2];

        int j = 0;
        for (int i = 0; i < commsize; i++, j++)
        {
            if (rank != i)
            {
                MPI_Irecv(rbuf + j * n, n, MPI_CHAR, i, 0, MPI_COMM_WORLD, &reqs[j]);
            }
            else
            {
                j--;
            }
        }
        for (int i = 0; i < commsize; i++, j++)
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

        MPI_Waitall(j, reqs, stat);
    //}

    time = MPI_Wtime() - time;

    printf("process %d: %s\n", rank, rbuf);

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