#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


int main(int argc, char * argv[])
{

MPI_Init(&argc,  &argv);

int comm_size;
MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

if(comm_size != 4)
{
    printf("This application is meant to be run with 4 MPI processes, not %d.\n", comm_size);
    MPI_Abort(MPI_COMM_WORLD,EXIT_FAILURE);
}

//Get my rank
int my_rank;

MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);


//Time to wait before processing, in seconds
const double waiting_time = 0.25;
int message = 12345;
double start;
double end;

if(my_rank == 0)
{

    start = MPI_Wtime();

    while(MPI_Wtime() - start < waiting_time)
    {
    //Loop until waiting time seconds have elapsed    
    }
    MPI_Send(&message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
}
else
{
    MPI_Recv(&message, 1, MPI_INT, my_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    start = MPI_Wtime();

    while(MPI_Wtime() - start < waiting_time)
    {
        //keep looping until waiting time seconds have elapsed
    }

    if(my_rank != comm_size -1)
    {
        MPI_Send(&message, 1, MPI_INT, my_rank + 1, 0, MPI_COMM_WORLD);
    }

}

MPI_Barrier(MPI_COMM_WORLD);
end = MPI_Wtime();

printf("[]MPI process %d] time elapsed during the job: %.2fs.\n" my_rank,end - start);

MPI_Finalize();

return EXIT_SUCCESS;

}
