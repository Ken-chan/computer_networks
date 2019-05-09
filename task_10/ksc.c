
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

int MARKER = 10, REQUEST = 4, PERMIT = 2;

void send_marker(int rank, int N)
{


}

int main(int argc, char** argv)
{
  int rank, N, tmp;
  MPI_Init(&argc, &argv); 
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &N);
  MPI_Status status;
  int i;
  if (rank == N-1)
  {
    send_marker(rank, N);
  }
  else
  {
    for (i = 0; i < 3; i++)
    {
      MPI_Send(&REQUEST, 1, MPI_INT,  LIDER, REQUEST, MPI_COMM_WORLD);
      MPI_Recv(&PERMIT, 1, MPI_INT, LIDER, PERMIT, MPI_COMM_WORLD, &status);
      printf("Process %d in critic section\n", rank);
      sleep(2);
      printf("Process %d out critic section\n", rank);
      MPI_Send(&RELIASE, 1, MPI_INT,  LIDER, RELIASE, MPI_COMM_WORLD);

      printf("Process %d in reminder section\n", rank);
      sleep(1);
      printf("Process %d out reminder section\n", rank);
    }
  }
  MPI_Finalize();
  return 0;
}
