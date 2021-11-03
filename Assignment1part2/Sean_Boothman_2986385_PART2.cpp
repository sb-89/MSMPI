#include <iostream>
#include <mpi.h>


void coordinator(int);
void participant(int);
int sum();
int sumDifferences();

int main(int argc, char* argv[]) {
	MPI_Init(NULL, NULL);

	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	if (world_rank == 0) {
		coordinator(world_size);
	}
	else {
		participant(world_size);
	}

	MPI_Finalize();
}

void coordinator(int world_size) {
	const int size = 1000;
	int rand_array[size];
	
	for (int i = 0; i < size; i++) {
		srand(1);
		rand_array[i] = rand() % 50;
	}

	int partition_size = size / world_size;
	MPI_Bcast(&partition_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
	for (int i = 1; i < world_size; i++) {
		MPI_Send(rand_array + (partition_size * i), partition_size, MPI_INT, i, 0, MPI_COMM_WORLD);
	}


}

void participant(int world_size)
{
	int partition_size = 0;
	MPI_Bcast(&partition_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

	int* partition = new int[partition_size];
	MPI_Recv(partition, partition_size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);




}

int sum()
{
	return 0;
}

int sumDifferences()
{
	return 0;
}
