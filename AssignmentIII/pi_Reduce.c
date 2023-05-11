
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

#define SEED     921
#define NUM_ITER 1000000000

int main(int argc, char* argv[])
{
    int rank, size, provided;
    int count_local = 0;
    int count_global = 0;
    double x, y, z, pi;
    double t1,t2,t3,t4;

    MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    t1=MPI_Wtime();
    
    srand(SEED*rank + 123456); // Important: Multiply SEED by "rank" when you introduce MPI!

    int NUM_local=NUM_ITER/size;

    // Calculate PI following a Monte Carlo method
    for (int iter = 0; iter < NUM_local; iter++)
    {
        // Generate random (X,Y) points
        x = (double)random() / (double)RAND_MAX;
        y = (double)random() / (double)RAND_MAX;
        z = sqrt((x*x) + (y*y));

        // Check if point is in unit circle
        if (z <= 1.0)
        {
            count_local++;
        }
    }
	
	t3=MPI_Wtime();
    
    MPI_Reduce(&count_local, &count_global, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	
	t4=MPI_Wtime();

    // Estimate Pi and display the result
    if(rank==0)
    {
        double NUM_tot=NUM_local*size;
        pi = ((double)count_global /NUM_tot) * 4.0;
        t2=MPI_Wtime();
        printf("Iteration tot: %f\n",NUM_tot);
        printf("Size: %d\n",size);
        printf("The result is %f\n", pi);
        printf("Time tot: %f\n", t2-t1);
		printf("Time communication: %f\n", t4-t3);
    }


    MPI_Finalize();

    return 0;
}

