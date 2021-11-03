#include <iostream>
#include <mpi.h>
// Distributed Systems - 2021
// Assignment 1 Part 1 - template
// Student Name - Student Number

#include <iostream>
#include <mpi.h>
#define MASTER 1 

// provided search method, do not need to change
int searchData(const int rank, const char* needle) {
	/* SEARCH METHOD
	Not interested in specifics therefore not shown
	This method examines the search space,
	and returns the number of hits found.
	*/
	srand(rank);
	int foundhits = rand() % 100;
	return foundhits;
}




int main(int argc, char** argv) {

	MPI_Init(NULL, NULL);

	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	
	int total_hits = 0;
	int value = 0;

	if (world_rank == 0) {

	}
	else {
		std::cout << "searching: " << std::endl;
		value = searchData(world_rank, argv[0]);
		std::cout << "World Rank: " << world_rank << " v: " <<  value << std::endl;
	}

	MPI_Reduce(&value, &total_hits, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	std::cout << "total: " << total_hits << std::endl;

	MPI_Finalize();

}
