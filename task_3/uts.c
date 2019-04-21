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
    MPI_Status status;
    int rank, N;
    MPI_Init(&argc, &argv); 
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &N);

    srandom(1 + rank);
    int time = random() % 41;

    int t0 = 0;
    if (!rank)
    {
        MPI_Status status;
        int p;
        for (p = 1; p < N; p++)
        {
            MPI_Recv(&t0, 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
            printf("Time %d, received from process %d in %d\n", t0, status.MPI_SOURCE, time);
            MPI_Send(&time, 1, MPI_INT,  status.MPI_SOURCE, 1, MPI_COMM_WORLD);
            time += random() % 41;
        }
    } else
    {
        int d_time = time;
        printf("Time %d, send from process %d\n", time, rank);
        MPI_Send(&time, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        time += random() % 41;
        MPI_Recv(&t0, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        printf("Time %d, received by process %d\n", t0, rank);
        printf("Correct time %.1f, on process %d\n", t0 + (float)(time - d_time) / 2,  rank);
    }
    MPI_Finalize();

    return 0;
}
