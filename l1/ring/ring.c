#include <stdio.h>
#include <malloc.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    int n = 1024;
    FILE *fp = fopen("time.txt", "w");
    fprintf(fp,"Message size: %dB\n\n", n);
    fclose(fp); 
    
    int rank, commsize;
    
    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    char *m = malloc(n * sizeof(char));
    char *m1 = malloc(n * sizeof(char));

    int right = (rank + 1) % commsize;
    int left = (rank - 1 + commsize) % commsize;

    double time = MPI_Wtime();
    for (int i = 0; i < commsize-1; i++)
    {
        MPI_Request reqs[2];
        MPI_Status stat[2];

        MPI_Irecv(m1, n, MPI_CHAR, left, 0, MPI_COMM_WORLD, &reqs[0]);
        MPI_Isend(m, n, MPI_CHAR, right, 0, MPI_COMM_WORLD, &reqs[1]);

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