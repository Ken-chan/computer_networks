// 8 proceses (2,4)
// 2 - mpiFinalize - 7,4
// 3 обнаруживает

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
//#define VOTE 3
//#define OK 4
//#define LIDER 2

int alarmflg = 1;
const int VOTE = 3, OK = 4, LIDER = 2;

void sighandler(int sig) {
   //printf("alarmflg set 0\n");
   alarmflg = 0;
}

void vote(int source, int N, int* arr) {
  int i;
  if (source == N-1)
    i = 0;
  else 
    i = source+1;
  MPI_Send(arr, N, MPI_INT,  i, VOTE, MPI_COMM_WORLD);
  printf("Process %d start alarm, %d\n", source, i);
  alarm(5);
}


int main(int argc, char** argv)
{
  int rank, N, tmp;
  MPI_Init(&argc, &argv); 
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &N);
  MPI_Status status;
  int data , lider_number = 7;
  int vote_flag = 0, end_flag = 1;
  int *array_for_active = calloc(N, sizeof (int) * N);
  signal(SIGALRM, &sighandler);
  int next = rank;
  if (rank == 7 || rank == 4)
  {
      MPI_Finalize();
      return 0 ;
  }
  if (rank == 3)
  {
    array_for_active[rank] = 1;
    vote(rank, N, array_for_active);
  }

  while (end_flag)
  {
    if (!alarmflg)
    {
      if (next == N-1)
        next = 0;
      else 
        next = next+1;
      vote(next, N, array_for_active);
      alarmflg = 1;
    }
    int read_flag = 0;
    MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &read_flag, &status);
    if (read_flag)
    {
      switch (status.MPI_TAG) 
      {
         case 2  :
            MPI_Recv(&tmp, 1, MPI_INT, MPI_ANY_SOURCE, 2, MPI_COMM_WORLD, &status);
            lider_number = tmp;
            printf("Process %d set lider %d\n", rank, lider_number);
            MPI_Finalize();
            return 0;

          case 3  :
            MPI_Recv(array_for_active, N, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            if (array_for_active[rank] == 1)
            {
              end_flag = 0;
              break;
            }
            array_for_active[rank] = 1;
            MPI_Send(&OK, 1, MPI_INT,  status.MPI_SOURCE, OK, MPI_COMM_WORLD);
            vote(rank, N, array_for_active);
            break; 
          
         case 4  :
            MPI_Recv(&tmp, 1, MPI_INT, MPI_ANY_SOURCE, 4, MPI_COMM_WORLD, &status);
            printf("Process %d recved OK from %d\n", rank, status.MPI_SOURCE);
            alarm(0);
            break; 

         default : 
          break;
        }
      }
  }
  int i;
  for (i = N-1; i >= 0; i--)
    if (array_for_active[i] == 1)
    {
      lider_number = i;
      break;
    }

  printf("Process %d set lider %d\n",rank, lider_number);
  for (i = 0; i < N; i++)
  {
      MPI_Send(&lider_number, 1, MPI_INT,  i, LIDER, MPI_COMM_WORLD);
  }
  MPI_Finalize();

  return 0;
}
