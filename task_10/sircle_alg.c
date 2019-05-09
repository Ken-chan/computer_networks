// circle algoritm with marker
// all process send and recv marcer in circle
// only process with marker can entry critic section

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
#include <signal.h>
#include <time.h>

int MARKER = 10, OK = 5, FIN_PREV = 2, FIN_PREV = 3;

void send_marker(int rank, int next, int N)
{
  MPI_Send(&MARKER, 1, MPI_INT,  next, MARKER, MPI_COMM_WORLD);
  MPI_Recv(&OK, 1, MPI_INT, next, OK, MPI_COMM_WORLD, &status);
  printf("Process %d send marker to \n", rank, next);
}

int main(int argc, char** argv)
{
  srand(time(NULL));
  int rank, N, tmp;
  MPI_Init(&argc, &argv); 
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &N);
  MPI_Status status;
  int i, next = rank + 1, prev = rank - 1, tasks = 3;
  if (rank == 0)
    prev = N-1;
  if (rank == N-1)
  {
    next = 0;
    send_marker(rank, next, N);
  }
  while (tasks)
  {
    int read_flag = 0;
    while (!read_flag)
      MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &read_flag, &status);
    if (status.MPI_TAG == FIN_NEXT)
    {
      MPI_Recv(&prev, 1, MPI_INT, status.MPI_SOURCE, MARKER, MPI_COMM_WORLD, &status);
      printf("Process %d set prev\n", prev);
    }
    if (status.MPI_TAG == FIN_NEXT)
    {
      MPI_Recv(&prev, 1, MPI_INT, status.MPI_SOURCE, MARKER, MPI_COMM_WORLD, &status);
      printf("Process %d set prev\n", prev);
    }
    MPI_Recv(&MARKER, 1, MPI_INT, prev, MARKER, MPI_COMM_WORLD, &status);
    printf("Process %d in critic section\n", rank);
    sleep(2);   //rand()%5;
    printf("Process %d out critic section\n", rank);
    MPI_Send(&RELIASE, 1, MPI_INT,  LIDER, RELIASE, MPI_COMM_WORLD);

    printf("Process %d in reminder section\n", rank);
    sleep(1);
    printf("Process %d out reminder section\n", rank);
  }

  printf("Process %d finished\n", rank);
  MPI_Send(&next, 1, MPI_INT,  prev, FIN_PREV, MPI_COMM_WORLD);
  MPI_Send(&prev, 1, MPI_INT,  next, FIN_NEXT, MPI_COMM_WORLD);
  MPI_Recv(&OK, 1, MPI_INT, next, OK, MPI_COMM_WORLD, &status);
  MPI_Recv(&OK, 1, MPI_INT, prev, OK, MPI_COMM_WORLD, &status);
  printf("Process %d out of circle \n", rank);
  MPI_Finalize();
  return 0;
}
