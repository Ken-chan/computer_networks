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

int main(int argc, char *argv[]){

    MPI_Status status;
    int rank, N, tmp_time, i, comand;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &N);
    int time = 0;
    
    char file_name[6];
    file_name[0] = (rank + 1) + '0';
    file_name[1] = '\0';
    strcat(file_name, ".dat");
    FILE *file = fopen(file_name, "r" );

    if (file)
    {
        while (fscanf(file, "%d", &comand) != EOF)
        {
            time ++;
            if (comand < 0)
            {
                MPI_Recv(&tmp_time, 1, MPI_INT, -comand - 1, 1, MPI_COMM_WORLD, &status);
                printf("Time %d recived by %d from %d, it's current time %d\n", tmp_time,  rank, -comand - 1, time);
                if (tmp_time + 1 > time)
                    time = tmp_time + 1;
                printf("Process %d now has time  %d\n", -comand - 1, time);
            }
            if (comand > 0)
            {
                MPI_Send(&time, 1, MPI_INT, comand - 1, 1, MPI_COMM_WORLD);
                printf("Process %d sent time %d to %d\n", rank, time, comand - 1);
            }
        }
        for (i = 0; i < N; i++)
            printf("Process %d has final time for process %d = %d\n", rank, i, time);
        fclose (file);
    }
    else
    {
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    MPI_Finalize();
    return 0;
}
