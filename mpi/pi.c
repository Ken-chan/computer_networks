#include "mpi.h"
#include <stdio.h>
#include <math.h>

double sircle( double x )
{
    return math.sqrt(4 - ax*x);
}

int main( int argc, char *argv[])
{
    int rank, size, numparts = 1000;
    double sum = 0, tmp_sum = 0, delta   = 1.0 / numparts, i = 0;


    int res;
    res = MPI_Init(&argc, &argv);
    if(res != MPI_SUCCESS)
    {
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    int rank = 0, size = 0;
    res = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if(res != MPI_SUCCESS)
    {
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    res = MPI_Comm_size(MPI_COMM_WORLD, &size);
    if(res != MPI_SUCCESS)
    {
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    while (i < 2 - size*delta )
    {       
            tmp_sum += (f(i) + f(i+delta)/2 * delta;
            i += delta * size;
    }
    MPI_Reduce(&tmp_sum, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0)
    {
        printf("pi = %f\n", sum);
    }
    MPI_Finalize();

    return 0;
}