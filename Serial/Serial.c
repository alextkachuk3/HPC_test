#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

void TestMonteCarlo(int iterations)
{
	clock_t start = clock();

	double x, y, z;
	int count = 0;

	for (int i = 0; i < iterations; i++)
	{
		x = (double)rand() / RAND_MAX;
		y = (double)rand() / RAND_MAX;
		z = sqrt((x * x) + (y * y));
		if (z <= 1)
		{
			count++;
		}
	}

	double pi = ((double)count / (double)iterations) * 4.0;

	clock_t end = clock();

	double duration = ((double) (end - start)) / CLOCKS_PER_SEC;

	printf("Iterations %i\nDuration %f\nPi: %f\n\n", iterations, duration, pi);
}

int main(int argc, char* argv[])
{
	srand((unsigned int)time(NULL));

	int iterations[] = { 100000, 1000000, 10000000, 100000000, 1000000000 };
	for (int i = 0; i < sizeof(iterations) / sizeof(int); i++)
	{
		TestMonteCarlo(iterations[i]);
	}
	return 0;
}
