#include <iostream>
#include <mpi.h>
#include <math.h>
#include <string>
#include <chrono>
//accept command line arg or default to 10

void coordinator(int);
void participant(int, int);
//sum each value in array
int sum(int*, int);
//sum of differences between standard dev and mean
int sumDifferences(int*, int, double);
void printArray(int*, int);
int standardDeviation(int, int);

//array size
int size = 0;

int main(int argc, char* argv[]) {
	//timer start
	auto start = std::chrono::steady_clock::now();
	//dynamic size
	if (argv[1])
		size = std::stoi(__argv[1]);
	else
		size = 10;
	
	MPI_Init(NULL, NULL);
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	if (world_rank == 0) {
		coordinator(world_size);
	}
	else {
		participant(world_rank, world_size);
	}

	MPI_Finalize();
	//timer end
	auto end = std::chrono::steady_clock::now();
	std::cout << "Time in milliseconds: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;
	std::cout << "Time in seconds: " << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << " seconds" << std::endl;
}

void coordinator(int world_size) {
	//dynamic allocation of size array
	int* rand_array = new int[size];
	srand(1);
	for (int i = 0; i < size; i++) {
		rand_array[i] = rand() % 50;
	}
	int partition_size = size / world_size;
	//broadcast partition sizes
	MPI_Bcast(&partition_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
	for (int i = 1; i < world_size; i++) {
		MPI_Send(rand_array + (partition_size * i), partition_size, MPI_INT, i, 0, MPI_COMM_WORLD);
	}

	double total = 0;
	for (int i = 0; i < partition_size; i++) {
		total += rand_array[i];
	}
	double average = (double)total / partition_size;
	std::cout << "Average of rank 0: " << average << std::endl;
	double total_average = 0;
	MPI_Reduce(&average, &total_average, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	total_average /= world_size;
	std::cout << "Total Average: " << total_average << std::endl;
	//broadcast total average to all the nodes
	MPI_Bcast(&total_average, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	// reduce sum of diff
	int sumOfDiff = sumDifferences(rand_array, partition_size, total_average);
	std::cout << "diff: " << sumOfDiff << "\n";
	int totalSumOfDiff = 0;
	MPI_Reduce(&sumOfDiff, &totalSumOfDiff, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	std::cout << "Total sum of diff: " << totalSumOfDiff << std::endl;
	//print
	printArray(rand_array, size);
	//sum
	int arr_sum = sum(rand_array, size);
	std::cout << "Total Sum: " << arr_sum << std::endl;
	//standard dev
	int standard_dev = standardDeviation(totalSumOfDiff, size);
	std::cout << "Standard Deviation: " << standard_dev << std::endl;
	delete[] rand_array;
}

void participant(int world_rank, int world_size)
{
	int partition_size = 0;
	MPI_Bcast(&partition_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

	int* partition = new int[partition_size];
	MPI_Recv(partition, partition_size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	double total = 0;
	for (int i = 0; i < partition_size; i++) {
		total += partition[i];
	}
	double average = (double)total / partition_size;
	std::cout << "Average for rank: " << world_rank << " is " << average << std::endl;
	double total_average = 0;
	//reduc average
	MPI_Reduce(&average, &total_average, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	
	// receive overall average from coordinator
	MPI_Bcast(&total_average, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	//sum of diff
	int sumOfDiff = sumDifferences(partition, partition_size, total_average);
	int totalSumOfDiff = 0;
	MPI_Reduce(&sumOfDiff, &totalSumOfDiff, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	delete[] partition;

}

int sum(int* arr, int arr_size)
{
	int sum = 0;
	for (int i = 0; i < arr_size; i++)
		sum += arr[i];
	return sum;
}

int sumDifferences(int* arr, int arr_size, double mean)
{
	int sum = 0;
	for (int i = 0; i < arr_size; i++) {
		sum += abs(pow(arr[i] - mean,2));
	}
	return sum;
}

void printArray(int* arr, int arr_size)
{
	for (int i = 0; i < arr_size; i++) {
		std::cout << "[" << i << "]: " << arr[i] << " ";
	}
	std::cout << std::endl;
}

int standardDeviation(int distance, int size)
{
	return sqrt(distance / size);
}
