#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <mpi.h>

double func(double x)
{
    return (1 - exp(0.7/x))/(2 + x);
}

const double eps = 1E-6;
const int n0 = 100;

int main(int argc, char **argv) //F(х)= (1 - eхp(0.7/х))/(2 + х), a = 1, b = 2
{
    double a = 1, b = 2;

    int rank, commsize;
    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n = n0, k;
    double sq[2], delta = 1;
    
    double time = MPI_Wtime();
    for (k = 0; delta > eps; n *= 2, k ^= 1)
    {
        int points_per_proc = n / commsize;
        int lb = rank * points_per_proc;
        int ub = (rank == commsize - 1) ? (n - 1) : (lb + points_per_proc - 1);

        double h = (b - a) / n;
        double s = 0.0;

        for (int i = lb; i <= ub; i++)
            s += func(a + h * (i + 0.5));

        MPI_Allreduce(&s, &sq[k], 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        sq[k] *= h;
        if (n > n0)
            delta = fabs(sq[k] - sq[k ^ 1]) / 3.0;
    }
    time = MPI_Wtime() - time;

    double max_time = 0.0;
	MPI_Reduce(&time, &max_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("%d %f\n", commsize, max_time);
    }

    MPI_Finalize();

    return 0;
}
