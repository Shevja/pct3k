#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

const double PI = 3.14159265358979323846;
const int n = 10000000;

double getrand() 
{ 
    return (double)rand() / RAND_MAX;
}
double func(double x, double y) 
{ 
    return pow(x, 4)/(0.5*pow(x, 2) + x + 6);
    // return 3 * pow(y, 2) * pow(sin(x), 2); 
}

int main(int argc, char **argv)
{
    int rank, commsize;
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    srand(rank);

    int in = 0;
    double s = 0;
    for (int i = rank; i < n; i += commsize)
    {
        double x = getrand() * PI; /* x in [0, pi] */
        double y = getrand();      /* y in [0, sin(x)] */
        if (y <= sin(x))
        {
            in++;
            s += func(x, y);
        }
    }

    int gin = 0;
    MPI_Reduce(&in, &gin, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    double gsum = 0.0;
    MPI_Reduce(&s, &gsum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        double v = PI * gin / n;
        double res = v * gsum / gin;
        printf("Result: %.12f, n %d\n", res, n);
    }
    MPI_Finalize();

    return 0;
}