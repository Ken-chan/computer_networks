#include "mpi.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


int main( int argc, char *argv[])
{
    int rank, size, numparts = 1000;

    int res;
    res = MPI_Init(&argc, &argv);
    res = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    res = MPI_Comm_size(MPI_COMM_WORLD, &size);
    double *u, *utmp, delta, maxdelta;
    double eps=1.e-6;
    double h, tau, tmp;

    MPI_Status status;

    int N = 1000;
    int i;
    int calc_num = 6;
    int first_arr_size = N/calc_num+N%calc_num+1;
    int arr_size = N/calc_num;

    if(rank == 0)
    {
     	arr_size = first_arr_size;
    }
    printf("(%d)\n", arr_size);

    if(rank == 0)
    {


        if((u = malloc((arr_size)*sizeof(double))) == NULL){
                printf("Can't allocate memory for u\n");
                exit(-1);
        }
	if((utmp = malloc((arr_size)*sizeof(double))) == NULL){
                printf("Can't allocate memory for utmp\n");
                free(u);
                exit(-1);
        }
	u[arr_size-1] = 0;
        utmp[arr_size-1] = 0;
    }
    else{

	if((u = malloc((arr_size)*sizeof(double))) == NULL){
                printf("Can't allocate memory for u\n");
                exit(-1);
        }


	if((utmp = malloc((arr_size)*sizeof(double))) == NULL){
                printf("Can't allocate memory for utmp\n");
                free(u);
                exit(-1);
        }

    }
    for(i=0; i<arr_size; i++){
        u[i] = 0;
        utmp[i] = 0;
    }
    if(rank == 0)
        utmp[0] = u[0] = 1.0;
    else
        utmp[0] = u[0] = 0.0;

    h = 1.0/N;
    tau = 0.5*(h*h);
    double tmp_node = 0;
    while(1){
    if(rank == 0)
        {
            for (i=1; i<arr_size-1; i++){
                utmp[i] = u[i]+(tau/(h*h))*(u[i-1]-2*u[i]+u[i+1]);
            }

            res = MPI_Send(&u[i], 1, MPI_DOUBLE, 1, 1, MPI_COMM_WORLD);
            res = MPI_Recv(&tmp_node, 1, MPI_DOUBLE, 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            //i+=1;
            utmp[i] = u[i]+(tau/(h*h))*(u[i-1]-2*u[i]+tmp_node);
        }
    if(rank != 0)
        {
            i = 0;
            res = MPI_Recv(&tmp_node, 1, MPI_DOUBLE, rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            utmp[i] = u[i]+(tau/(h*h))*(tmp_node-2*u[i]+u[i+1]);

            res = MPI_Send(&u[i], 1, MPI_DOUBLE, rank-1, 1, MPI_COMM_WORLD);
            for (i=1; i<arr_size-1; i++){
                utmp[i] = u[i]+(tau/(h*h))*(u[i-1]-2*u[i]+u[i+1]);
            }
            if(rank != size-1)
            {
                res = MPI_Send(&u[i], 1, MPI_DOUBLE, rank+1, 1, MPI_COMM_WORLD);
                res = MPI_Recv(&tmp_node, 1, MPI_DOUBLE, rank+1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                utmp[i] = u[i]+(tau/(h*h))*(u[i-1]-2*u[i]+tmp_node);
            }
    }











maxdelta = 0;
        for (i=1; i<arr_size; i++){
            delta = fabs(utmp[i]-u[i]);
            if(delta > maxdelta) maxdelta = delta;
        }

    MPI_Allreduce(MPI_IN_PLACE, &maxdelta, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
        if(maxdelta < eps) break;

        for(i = 0; i < arr_size; i++){
            u[i] = utmp[i];
        }
    if(rank == 0){
            u[arr_size-1] = utmp[arr_size-1];
        }
    }
    if(rank != 0)
    {
        res = MPI_Send(utmp, arr_size, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
    }
    if (rank == 0)
    {

        FILE *ff;
        if ((ff = fopen("resfile", "w+")) == NULL)
        {
            printf("Can't open file\n"); free(u); free(utmp);
            exit(-1);
        }
    int k = 0;
        int i = 0;
        for(i; i<arr_size; i++)
           fprintf(ff, "%f ", utmp[i]);
        for(k = 1; k < size; k++)
        {
            res = MPI_Recv(utmp, N/calc_num, MPI_DOUBLE, k, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            for(i=0; i<N/calc_num; i++)
               fprintf(ff, "%f ", utmp[i]);
       }

        fclose(ff); free(u); free(utmp);
    }

    MPI_Finalize();

    return 0;
}