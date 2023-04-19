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
		int j;
		int lp=9;
		int p[]={1,2,4,8,16,20,24,28,32};
		
		double start_time, run_time;
		
		for (i = 0; i < STREAM_ARRAY_SIZE; i++) 
		{
			x[i] = rand() / (double)(RAND_MAX);
		}
		
		for(j=0;j<lp; j++)
		{
		double sum_val = 0.0;
		omp_set_num_threads(p[j]);
		start_time=omp_get_wtime();
		#pragma omp parallel
		{
			#pragma omp for
			for (i = 0; i < STREAM_ARRAY_SIZE; i++) 	
			{
				#pragma omp critical 
				{
					sum_val += x[i];
			}	}
		}
		
		run_time=omp_get_wtime() - start_time;
		printf("Number threads: %d \n", p[j]);
		printf("Sum: %f \n", sum_val);
		printf("Run time: %f \n", run_time);
		}
		return 0;
	}