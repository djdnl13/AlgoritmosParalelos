#include <stdio.h>
#include <mpi.h>


int main(int argc, char **argv)
{
    int i, envia, recibe;
    int procs, miRank;
    int tag = 10;

    MPI_Status status;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &miRank);

    if(miRank < procs-1)
    {
            MPI_Recv(&recibe, 1, MPI_INT, miRank, tag, MPI_COMM_WORLD, &status);
            printf("Recibido el valor %d en ell proceso %d\n", recibe, miRank);
    }
    else
    {
	printf("Ingrese un numero  :");
	scanf("%d", &envia);

        MPI_Rsend(&envia, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;

}
