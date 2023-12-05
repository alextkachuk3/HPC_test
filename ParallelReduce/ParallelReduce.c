#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include <math.h>

#define SEED 354853899

int main(int argc, char* argv[])
{
	long iterations = 10000000;

	double x, y, z;
	int count = 0;
	double pi;

	int reducedcount;

	int process_count;
	int process_rank;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &process_count);

	iterations /= process_count;

	srand(SEED + process_rank);

	for (int i = 0; i < iterations; ++i)
	{
		x = (double)rand() / RAND_MAX;
		y = (double)rand() / RAND_MAX;
		z = sqrt((x * x) + (y * y));
		if (z <= 1)
		{
			count++;
		}
	}

	MPI_Reduce(&count, &reducedcount, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (process_rank == 0)
	{
		pi = ((double)reducedcount / (double)iterations) * 4.0;
		printf("Pi: %f\n", pi);
	}

	MPI_Finalize();
	return 0;
}