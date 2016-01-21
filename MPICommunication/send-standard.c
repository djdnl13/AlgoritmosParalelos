/*
A simple MPI example program using standard mode send and receive

The program consists of two processes. Process 0 sends a message
to the receiver. This receives the message and sends it back.

Compile the program with 'mpicc -O3 send-standard.c -o send-standard'
Run it on two processes.
*/

#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"

int main(int argc, char* argv[]) {
  int x, y, np, me;
  int tag = 42;
  MPI_Status status;

  /* Initialize MPI */
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &np);
  MPI_Comm_rank(MPI_COMM_WORLD, &me);

  /* Check that we run on exactly two proceses */
  if (np != 2) {
    if (me == 0) {
      printf("You have to use exactly 2 processes to run this program\n");
    }
    MPI_Finalize();	       /* Quit if there is only one process */
    exit(0);
  }
  
  x = 12345; y = me;   /* Initialize */
  if (me == 0) {

    printf("Process %d sending to process 1\n", me);    
    MPI_Send(&x, 1, MPI_INT, 1, tag, MPI_COMM_WORLD);
    printf("Process %d receiving from process 1\n", me);
    MPI_Recv (&y, 1, MPI_INT, 1, tag, MPI_COMM_WORLD, &status);
    printf ("Process %d received value %d\n", me, y);
    
  } else { /* me == 1 */
    
    MPI_Recv (&y, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
    MPI_Send (&y, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
  }
  
  MPI_Finalize();
  exit(0);
}
