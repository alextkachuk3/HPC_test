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

	int process_count;
	int process_rank;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &process_count);

	iterations /= process_count;

	int* recieved = (int*)calloc(process_count, sizeof(int));

	srand(SEED + process_rank);

	if (process_rank != 0)
	{
		for (int i = 0; i < iterations; ++i)
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
		for (int i = 0; i < iterations; ++i)
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

		pi = ((double)finalcount / (double)(process_count * iterations)) * 4.0;
		printf("Pi: %f\n", pi);
	}

	MPI_Finalize();
	free(recieved);

	return 0;
}