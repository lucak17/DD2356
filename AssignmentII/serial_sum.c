#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

#ifndef STREAM_ARRAY_SIZE
#   define STREAM_ARRAY_SIZE	10000000
#endif

#ifndef STREAM_TYPE
#define STREAM_TYPE double
#endif

static STREAM_TYPE	x[STREAM_ARRAY_SIZE];

int main(int argc, char *argv[]) {
	

	size_t i;
	double sum_val = 0.0;
	double start_time, run_time;
	
	for (i = 0; i < STREAM_ARRAY_SIZE; i++) 
	{
		x[i] = rand() / (double)(RAND_MAX);
	}

	start_time=omp_get_wtime();
   //#pragma omp parallel
    {
		for (i = 0; i < STREAM_ARRAY_SIZE; i++) 	
		{
			sum_val += x[i];
		}
    }
	
	run_time=omp_get_wtime() - start_time;
	printf("Sum: %f \n", sum_val);
	printf("Run time: %f \n", run_time);
    return 0;
}