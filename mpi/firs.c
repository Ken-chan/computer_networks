#include <stdio.h>
#include <mpi.h>
int main(int argc, char *argv[])
{
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
	int mes = 0;
        struct MPI_Status status;
        if(rank == 0)
        {
                res = MPI_Send(*mes, 1, MPI_INT, rank+1, 1, MPI_COMM_WORLD);
                if(res != MPI_SUCCESS)
                {
                        MPI_Abort(MPI_COMM_WORLD, 1);
                }
                res = MPI_Recv(*mes, 1, MPI_INT, size-1, MPI_ANY_TAG, MPI_COMM_WORLD, *status);
                if(res != MPI_SUCCESS)
                {
                        MPI_Abort(MPI_COMM_WORLD, 1);
                }
        }
        else
        {
                res = MPI_Recv(*mes, 1, MPI_INT, rank-1, MPI_ANY_TAG, MPI_COMM_WORLD, *status);
                if(res != MPI_SUCCESS)
                {
                        MPI_Abort(MPI_COMM_WORLD, 1);
                }
                mes+=rank;
                res = MPI_Send(*mes, 1, MPI_INT, rank+1, 1, MPI_COMM_WORLD);
                if(res != MPI_SUCCESS)
                {
                        MPI_Abort(MPI_COMM_WORLD, 1);
                }
        }
        if(rank == size-1)
        {
                printf("%s\n",mes);
        }
        MPI_Finalize();
}