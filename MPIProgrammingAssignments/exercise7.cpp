#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>


int main() {
  double max, min, *vector, width;
  int comm_sz, i, my_rank;
  int n = 0;

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  if(comm_sz != 2)
  {
     MPI_Finalize();
     return 0;
  }

    if(my_rank == 0)
    { 
 	for(i=0 ; i<5 ; ++i)
	{
	      MPI_Send(&n, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
	      printf("Sent to process 1, value %d \n", n);
	      ++n;     
	      MPI_Recv(&n, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	      printf("Received from process 1, value %d \n", n);
	}
    }
    else
    {
	for(i=0 ; i<5 ; ++i)
	{
	      MPI_Recv(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	      printf("Received from process 0, value %d \n", n);
	      n++;
	      MPI_Send(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	      printf("Sent to process 0, value %d \n", n);
	}
    }

  MPI_Finalize();

  return 0;
}
