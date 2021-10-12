void coordinator();
void computeAverage();

int COMPUTE_AVERAGE = 1;
int world_size;
int world_rank;


#include <iostream>
#include <cstdlib>
#include <mpi.h>



int main(int argc, char** argv) {
	MPI_Init(NULL, NULL);

	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);


	if (world_rank == 0)
		coordinator();
	else
		computeAverage();
}


void coordinator() {

}
void computeAverage() {

}