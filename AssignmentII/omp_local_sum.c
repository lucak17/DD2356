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
		int lp=4;
		int p[]={1,32,64,128};
		
		double start_time, run_time;
		
		for (i = 0; i < STREAM_ARRAY_SIZE; i++) 
		{
			x[i] = rand() / (double)(RAND_MAX);
		}
		
		for(j=0;j<lp; j++)
		{
		double local_sum[p[j]];
		double sum_val = 0.0;
		
		for(int k=0;k<p[j];k++)
		{
			local_sum[k]=0;
		}
		omp_set_num_threads(p[j]);
		start_time=omp_get_wtime();
		#pragma omp parallel shared(local_sum)
		{
			int id=omp_get_thread_num();
			#pragma omp for
			for (i = 0; i < STREAM_ARRAY_SIZE; i++) 	
			{
				local_sum[id]+=x[i];
			}
		}
		
		for(int k=0;k<p[j];k++)
		{
			sum_val+=local_sum[k];
		}
		
		run_time=omp_get_wtime() - start_time;
		printf("Number threads: %d   ", p[j]);
		printf("Sum: %f   ", sum_val);
		printf("Run time: %f \n", run_time);
		}
		return 0;
	}