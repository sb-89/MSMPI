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

	MPI_Finalize();
}


void coordinator() {
	int message;

	MPI_Send(&COMPUTE_AVERAGE, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);

	MPI_Recv(&message, 1, MPI_INT, world_size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	srand(world_rank);

	int sum = 0;
	int count = 100000;

	for (int i = 0; i < count; i++) {
		sum += rand() % 10;
	}

	float average = sum / (float)count;

	std::cout << "cooridinator " << world_rank << " sum is: " << sum << std::endl;
	std::cout << "cooridinator " << world_rank << " average is: " << average << std::endl;

	MPI_Send(&average, 1, MPI_FLOAT, 1, 0, MPI_COMM_WORLD);
	MPI_Recv(&average, 1, MPI_FLOAT, world_size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	std::cout << "coordinator " << world_rank << " total average is: " << average / world_size
		<< std::endl;
	
}
void computeAverage() {
	int message;
	float current_average;

	MPI_Recv(&message, 1, MPI_INT, (world_rank + world_size - 1) % world_size, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	MPI_Send(&COMPUTE_AVERAGE, 1, MPI_INT, (world_rank + 1) % world_size, 0, MPI_COMM_WORLD);



}