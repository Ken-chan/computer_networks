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

int MARKER = 10, OK = 5, FIN_PREV = 2, FIN_NEXT = 3;
int tasks = 3, critic_section_flag = 0;
// critic_section_flag  :
    // 2 - ready for critic section, 
    // 0 - ready for work, 
    // 1 - in reminder section

void sighandler(int sig) 
{
  printf("Process out of reminder section\n");
  critic_section_flag = 2;
}

void send_marker(int rank, int next, int N)
{
  MPI_Send(&MARKER, 1, MPI_INT,  next, MARKER, MPI_COMM_WORLD);
  //MPI_Recv(&OK, 1, MPI_INT, next, OK, MPI_COMM_WORLD, &status);
  //printf("Process %d send marker to \n", rank, next);
}

int main(int argc, char** argv)
{
  srand(time(NULL));
  int rank, N, tmp;
  MPI_Init(&argc, &argv); 
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &N);
  MPI_Status status;
  int next = rank + 1, prev = rank - 1;
  signal(SIGALRM, &sighandler);
  
  if (rank == 0)
    prev = N-1;
  if (rank == N-1)
  {
    next = 0;
    send_marker(rank, next, N);
  }

  while (tasks)
  {
    if (critic_section_flag == 0)
    {
      critic_section_flag = 1;
      printf("Process %d in reminder section\n", rank);
      alarm(3);
    }

    int read_flag = 0;
    MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &read_flag, &status);
    if (read_flag)
    {
      if (status.MPI_TAG == FIN_NEXT)
      {
        MPI_Recv(&prev, 1, MPI_INT, status.MPI_SOURCE, FIN_NEXT, MPI_COMM_WORLD, &status);
        MPI_Send(&OK, 1, MPI_INT,  status.MPI_SOURCE, OK, MPI_COMM_WORLD);
        printf("Process %d set prev %d\n", rank, prev);
      }
      if (status.MPI_TAG == FIN_PREV)
      {
        MPI_Recv(&next, 1, MPI_INT, status.MPI_SOURCE, FIN_PREV, MPI_COMM_WORLD, &status);
        MPI_Send(&OK, 1, MPI_INT,  status.MPI_SOURCE, OK, MPI_COMM_WORLD);
        printf("Process %d set next %d\n", rank, next);
      }
      if (status.MPI_TAG == MARKER)
      {
        MPI_Recv(&MARKER, 1, MPI_INT, status.MPI_SOURCE, MARKER, MPI_COMM_WORLD, &status);
        //MPI_Send(&OK, 1, MPI_INT,  status.MPI_SOURCE, OK, MPI_COMM_WORLD);
        //printf("Process %d recv marker from %d \n", rank, status.MPI_SOURCE);
        if (critic_section_flag == 2)
        {
          printf("Process %d in critic section \n", rank);
          sleep(2);
          printf("Process %d out of critic section \n", rank);
          critic_section_flag = 0;
          tasks --;
          printf("Process %d tasks %d \n",rank, tasks); 
          if (tasks == 0)
          {
            printf("Process %d finished\n", rank);
            if (next != rank)
            {
              MPI_Send(&next, 1, MPI_INT,  prev, FIN_PREV, MPI_COMM_WORLD);
              MPI_Send(&prev, 1, MPI_INT,  next, FIN_NEXT, MPI_COMM_WORLD);
              MPI_Recv(&OK, 1, MPI_INT, next, OK, MPI_COMM_WORLD, &status);
              MPI_Recv(&OK, 1, MPI_INT, prev, OK, MPI_COMM_WORLD, &status);
            }
          }
        }  
        send_marker(rank, next, N);
      }
    }
  }

  printf("Process %d out of circle \n", rank);
  MPI_Finalize();
  return 0;
}
