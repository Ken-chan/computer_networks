#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

int main(int argc, char** argv)
{
    int rank, N;
    MPI_Init(&argc, &argv); 
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &N);

    srandom(1 + rank);

    MPI_Status status;
    if (!rank)
    {
        int t0 = random() % 41;
        int p, del;
        int S_del = 0;
        int* array = calloc(N, sizeof(int));
        printf("Time %d, on zero process\n", t0);
        for (p = 1; p < N; p++)
        {
            MPI_Send(&t0, 1, MPI_INT,  p, 1, MPI_COMM_WORLD);
            MPI_Recv(&del, 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
            printf("DeltaTime %d, received from process %d\n", del, status.MPI_SOURCE);
            S_del += del;
            array[p] = del;
        }
        S_del /= N;
        printf("AverageDeltaTime %d\n", S_del);
        int p_del;
        for (p = 1; p < N; p++)
        {
            p_del = array[p] - S_del;
            MPI_Send(&p_del, 1, MPI_INT,  p, 1, MPI_COMM_WORLD);
        }
        printf("Final Time %d, on process 0 \n", t0 + S_del);
    } else
    {
        int flag = 0;
        int t1 = random() % 41;
        int t0;
        while(!flag)
            MPI_Iprobe(0, 1, MPI_COMM_WORLD, &flag, &status);
        printf("Time %d, on process %d\n", t1, rank);
        MPI_Recv(&t0, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        int del = t1 - t0;
        MPI_Send(&del, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        MPI_Recv(&del, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        t1 -= del;
        printf("Finel Time %d, on process %d\n", t1, rank);
    }
    MPI_Finalize();

    return 0;
}
