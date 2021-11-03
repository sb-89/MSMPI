

#include <iostream>	
#include <mpi.h>

int world_rank;
int world_size;

int main(int argc, char* argv[]) {
	MPI_Init(NULL, NULL);

	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	std::cout << "My rank is: " << world_rank << std::endl;
	std::cout << argv[0] << std::endl;
	MPI_Finalize();

}