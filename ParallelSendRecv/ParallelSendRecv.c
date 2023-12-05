#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include <math.h>

#define SEED 354853899

int process_count;
int process_rank;

void TestMonteCarlo(int iterations)
{
	double start = MPI_Wtime();

	double x, y, z;
	int count = 0;
	double pi;
	
	int process_iterations = iterations / process_count;

	int* recieved = (int*)calloc(process_count, sizeof(int));	

	if (process_rank != 0)
	{
		for (int i = 0; i < process_iterations; ++i)
		{
			x = (double)rand() / RAND_MAX;
			y = (double)rand() / RAND_MAX;
			z = sqrt(x * x + y * y);
			if (z <= 1)
			{
				count++;
			}
		}
		MPI_Send(&count, 1, MPI_INT, 0, process_rank, MPI_COMM_WORLD);
	}
	else if (process_rank == 0)
	{
		for (int i = 0; i < process_iterations; ++i)
		{
			x = (double)rand() / RAND_MAX;
			y = (double)rand() / RAND_MAX;
			z = sqrt(x * x + y * y);
			if (z <= 1)
			{
				recieved[0]++;
			}
		}
		for (int i = 1; i < process_count; i++)
		{
			MPI_Recv(&recieved[i], process_count, MPI_INT, MPI_ANY_SOURCE, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
	}

	if (process_rank == 0)
	{
		int finalcount = 0;
		long finalniter = 0;
		for (int i = 0; i < process_count; ++i)
		{
			finalcount += recieved[i];
		}

		double finish = MPI_Wtime();
		double duration = finish - start;

		pi = ((double)finalcount / (double)(process_count * process_iterations)) * 4.0;
		printf("Iterations %i\nDuration %f\nPi: %f\n", iterations, duration, pi);
	}

	free(recieved);
}

int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &process_count);
	setvbuf(stdout, 0, _IONBF, 0);

	srand(SEED + process_rank);

	int iterations[] = { 100000, 1000000, 10000000, 100000000, 1000000000 };
	for (int i = 0; i < sizeof(iterations) / sizeof(int); i++)
	{
		TestMonteCarlo(iterations[i]);
	}

	MPI_Finalize();

	return 0;
}