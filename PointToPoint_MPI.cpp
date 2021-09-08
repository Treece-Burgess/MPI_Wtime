#include <mpi.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;


//argc is the amount of arguments while argv is the actual arguments
int main(int argc, char *argv[])
{

//& gives the address/passing addresses of argc and argv
//MPI_COMM_WORLD is set up at the begining of every parallel job by MPI_Init

MPI_Init(&argc, &argv);

int count = 10;
double xsend[count], xrecv[count];
for(int i = 0; i < count; i++){
    xsend[i] = (double)i;
}
//rank - unique, portable identifier to distinguish the individual process within the set of processes
//process - an indpedent unit of computation that has ownership of a portion of memory and control over resours in user space
int rank, nprocs;
//gets the rank number of the process
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//gets the number of ranks in the program determined by the mpirun command
MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

if(nprocs%2 == 0){
    if(rank == 0){
        cout << "Must be called with an even number of processes" << endl;
    }
    exit(1);
}

int tag = rank/2; //integer division pairs up the tags for the send and receive partners
int partner_rank = (rank/2) * 2 + (rank+1) % 2; // partner rank is the opposite member of the pair
MPI_Comm comm = MPI_COMM_WORLD; //space in which things will communicate with each other, everything in the whole world can talk to each other.
//defines an array of requests and sets to null so these are defined when tested for completion
MPI_Request[2] = {MPI_REQUEST_NULL, MPI_REQUEST_NULL};


MPI_Irecv(xrecv, count, MPI_DOUBLE, partner_rank, tag, comm, &requests[0]);
MPI_Isend(xsend, count, MPI_DOUBLE, partner_rank, tag, comm, &requests[1]);
//Calls a Waitall to wait for the send and receive to complete
MPI_Waitall(2, requests, MPI_STATUSES_IGNORE);

if(rank == 0) cout << "SendRecv successfully completed" << endl;

MPI_Finalize();
return 0;




}
