#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc, char *argv[])
{


int rank, nprocs, ncells = 100000;

MPI_Init(&argc, &argv);
MPI_Comm comm = MPI_COMM_WORLD;
MPI_Comm_rank(comm, &rank);
MPI_Comm_size(comm, &nprocs);
double MPI_Wtime();
//Section helps compute the size of the array on every process, ncells is telling how many values are in the array
long ibegin = ncells * (rank)/nprocs;
long iend   = ncells * (rank+1)/nprocs;
int nsize   = (int)(iend-ibegin);


double *a_global, *a_test;
if(rank == 0){
    //Sets up data on the main process
    a_global = (double *) malloc(ncells*sizeof(double));
    for(int i = 0; i < ncells; i++){
        a_global[i] = (double)i;
    }

}
//Section gets the sizes and offsets into global arrays for communication
int nsizes[nprocs], offsets[nprocs];
MPI_Allgather(&nsize, 1, MPI_INT, nsizes, 1, MPI_INT, comm);
offsets[0] = 0;
for(int i = 1; i < nprocs; i++){
    offsets[i] = offsets[i-1] + nsizes[i-1];
}
//Section distributes the data onto the other processes
double *a = (double *) malloc(nsize*sizeof(double)); //type casting the function malloc
    MPI_Scatterv(a_global, nsizes, offsets, MPI_DOUBLE, a, nsize, MPI_DOUBLE, 0, comm);

//Section does the computation

for(int i = 0; i < nsize; i++){
    a[i] += 1.0;
}
//Section returns array data to the main process, perhaps for output
if(rank == 0){
a_test = (double *) malloc(ncells*sizeof(double));
}

MPI_Gatherv(a,nsize, MPI_DOUBLE, a_test, nsizes, offsets, MPI_DOUBLE, 0, comm);

if(rank == 0){
    int ierror = 0;
    for(int i = 0; i < ncells; i++){
        if(a_test[i] != a_global[i] + 1.0){
            printf("Error: index %d a_test %lf a_global %lf\n",i,a_test[i],a_global[i]);
            ierror++;
        }

  }
    printf("Report: Correct results %d errors %d\n",ncells-ierror,ierror);
}

free(a);
if(rank == 0){
    free(a_global);
    free(a_test);
}

MPI_Finalize();
return 0;

}
  
