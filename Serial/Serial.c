#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char* argv[])
{
    int niter = 10000000;

    double x, y, z;
    int count = 0;

    srand((unsigned int)time(NULL));

    for (int i = 0; i < niter; i++)
    {
        x = (double)rand() / RAND_MAX;
        y = (double)rand() / RAND_MAX;
        z = sqrt((x * x) + (y * y));
        if (z <= 1)
        {
            count++;
        }
    }

    double pi = ((double)count / (double)niter) * 4.0;

    printf("Pi: %f\n", pi);

    return 0;
}