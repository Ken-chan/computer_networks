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
int VOTE = 3, OK = 4, LIDER = 2;

void sighandler(int sig) {
   printf("set 0\n");
   alarmflg = 0;
}

void vote(int source, int N) {
    int i;
    for (i = source + 1; i < N; i++)
    {
        MPI_Send(&VOTE, 1, MPI_INT,  i, 1, MPI_COMM_WORLD);

        printf("Process %d start alarm, %d\n", source, i);
    }
    alarm(5);
}


int main(int argc, char** argv)
{
    int rank, N;
    MPI_Init(&argc, &argv); 
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &N);
    MPI_Status status;
    int data , lider_number = 7;
    int vote_flag = 0;
    signal(SIGALRM, &sighandler);
    if (rank == 7 || rank == 4)
    {
        MPI_Finalize();
        return 0 ;
    }
    if (rank == 3)
        vote(rank, N);

    while(alarmflg)
    {
        int flag = 0;
        while(!flag && alarmflg)
            MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
        if (!alarmflg)
            break;
        MPI_Recv(&data, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        printf("Process %d recved %d\n", rank, data);
        switch(data) {

           case 2  :
              lider_number = status.MPI_SOURCE;
              printf("Process %d set lider %d\n", rank, lider_number);
              MPI_Finalize();
              return 0;

            case 3  :
              MPI_Send(&OK, 1, MPI_INT,  status.MPI_SOURCE, 1, MPI_COMM_WORLD);
              if (vote_flag == 0){
                  printf("Process %d start vote\n", rank);
                  vote(rank, N);
                  vote_flag = 1;    
              }
              break; 
            
           case 4  :
              printf("Process %d recved OK from %d\n", rank, status.MPI_SOURCE);
              alarm(0);
              break; 

           default : 
            break;
        }
    }
    lider_number = rank;

    printf("set lider %d\n", lider_number);
    int i;
    for (i = 0; i < N, i != lider_number, i != 4, i != 7; i++)
    {
        MPI_Send(&LIDER, 1, MPI_INT,  i, 1, MPI_COMM_WORLD);
    }
    MPI_Finalize();

    return 0;
}