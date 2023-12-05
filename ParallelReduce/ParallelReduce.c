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
	long long count = 0;
	double pi;

	long long reduced_count;

	int process_iterations = iterations / process_count;

	for (int i = 0; i < process_iterations; i++)
	{
		x = (double)rand() / RAND_MAX;
		y = (double)rand() / RAND_MAX;
		z = sqrt(x * x + y * y);
		if (z <= 1)
		{
			count++;
		}
	}

	MPI_Reduce(&count, &reduced_count, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

	if (process_rank == 0)
	{
		pi = ((double)reduced_count / (double)(process_count * process_iterations)) * 4.0;

		double finish = MPI_Wtime();
		double duration = finish - start;

		printf("Iterations %i\nDuration %f\nPi: %f\n\n", iterations, duration, pi);
	}
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
