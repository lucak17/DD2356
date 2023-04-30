#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[])
{

    int rank, size, i, provided;

    // number of cells (global)
    int nxc = 128; // make sure nxc is divisible by size
    double L = 2*3.1415; // Length of the domain
    double *df_true = calloc(nxc+1,sizeof(double));
    double dxx = L / ((double) nxc);

    for (i=0; i<(nxc+1); i++)
    {
        df_true[i]=cos(i*dxx);
    }


    MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // number of nodes (local to the process): 0 and nxn_loc-1 are ghost cells
    int nxc_loc= nxc/size;
    int nxn_loc = nxc/size + 3; // number of nodes is number cells + 1; we add also 2 ghost cells
    double L_loc = L/((double) size);
    double dx = L / ((double) nxc);
    int next, prev;

    // define out function
    double *f = calloc(nxn_loc, sizeof(double)); // allocate and fill with z
    double *dfdx = calloc(nxn_loc, sizeof(double)); // allocate and fill with z

    for (i=1; i<(nxn_loc-1); i++)
        f[i] = sin(L_loc*rank + (i-1) * dx);


    // need to communicate and fill ghost cells f[0] and f[nxn_loc-1]
    // communicate ghost cells
    next=rank+1;
    prev=rank-1;
    if(rank==(size-1))
    {
        next=0;
    }
    if(rank==0)
    {
        prev=size-1;
    }
    MPI_Send(&f[nxn_loc-3],1,MPI_DOUBLE,next,0,MPI_COMM_WORLD);
    MPI_Recv(&f[0],1,MPI_DOUBLE,prev,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

    MPI_Send(&f[2],1,MPI_DOUBLE,prev,0,MPI_COMM_WORLD);
    MPI_Recv(&f[nxn_loc-1],1,MPI_DOUBLE,next,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

// here we finish the calculations

// calculate first order derivative using central difference
// here we need to correct value of the ghost cells!
    for (i=1; i<(nxn_loc-1); i++)
        dfdx[i] = (f[i+1] - f[i-1])/(2*dx);


// Print f values

    if (rank==0)  // print only rank 0 for convenience
    {
        printf("My rank %d of %d\n", rank, size );
        printf("Here are my values for f including ghost cells\n");
        for (i=0; i<nxn_loc; i++)
            printf("%f\n", f[i]);
        printf("\n");
    }

    printf("My rank %d of %d\n", rank, size );
    printf("Values sin(x) at local nodes 0, 1, 2: %f, %f, %f, \n", f[0],f[1],f[2]);
    printf("Values sin(x) at local nodes nxn_loc-3, nxn_loc-2, nxn_loc-1: %f, %f, %f, \n", f[nxn_loc-3],f[nxn_loc-2],f[nxn_loc-1]);
    printf("Numerical derivative at local nodes 1, nxn_loc-2: %f, %f \n", dfdx[1],dfdx[nxn_loc-2]);
    printf("Exact derivative at local nodes 1, nxn_loc-2: %f, %f \n\n", df_true[rank*nxc_loc], df_true[(rank+1)*nxc_loc]);

    free(df_true);
    free(f);
    free(dfdx);

    MPI_Finalize();
}

