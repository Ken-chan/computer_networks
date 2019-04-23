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
    int rank, N, i, comand;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &N);
    int *time = calloc(N, sizeof (int) * N);
    int *recive_time = calloc(N, sizeof (int) * N);
    
    char file_name[7];
    file_name[0] = 'v';
    file_name[1] = (rank + 1) + '0';
    file_name[2] = '\0';
    strcat(file_name, ".dat");
    FILE *file = fopen(file_name, "r" );

    if (file)
    {
        while (fscanf(file, "%d", &comand) != EOF)
        {
            time[rank] ++;
            if (comand < 0)
            {
                MPI_Recv(recive_time, N, MPI_INT, -comand - 1, 1, MPI_COMM_WORLD, &status);
                if (N == 3)
                    printf("Time [%d %d %d] recived by %d from %d\n", recive_time[0], recive_time[1], recive_time[2],  rank, -comand - 1);
                for (i = 0; i < N; i++)
                    if (recive_time[i] > time[i])
                        time[i] = recive_time[i];
                if (N == 3)
                    printf("Process %d now has time  [%d %d %d]\n", rank, time[0], time[1], time[2]);
            }
            if (comand > 0)
            {
                MPI_Send(time, N, MPI_INT, comand - 1, 1, MPI_COMM_WORLD);
                if (N == 3)
                    printf("Process %d sent time [%d %d %d] to %d\n", rank, time[0], time[1], time[2], comand - 1);
            }
        }
        for (i = 0; i < N; i++)
            printf("Process %d has final time for process %d = %d\n", rank, i, time[i]);
        fclose (file);
    }
    else
    {
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    free(time);
    free(recive_time);
    MPI_Finalize();
    return 0;
}
