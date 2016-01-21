/*
A simple MPI example program using buffered send.

The program consists of two processes. Process 0 sends a message
to the receiver. This receives the message and sends it back.

Compile the program with 'mpicc send-buffered.c -o send-buffered'
Run the program on two processes.
*/

#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"

#define BUFFSIZE 10240    /* Size of the message buffer */

int main(int argc, char* argv[]) {
  int x, y, np, me;
  int buff[BUFFSIZE];   /* Buffer to be used in the communication */
  int size = BUFFSIZE;
  int tag = 42;
  MPI_Status status;

  MPI_Init(&argc, &argv);              /* Initialize MPI */
  MPI_Comm_size(MPI_COMM_WORLD, &np);  /* Get number of processes */
  MPI_Comm_rank(MPI_COMM_WORLD, &me);  /* Get own identifier */

  /* Check that we run on exactly two processors */
  if (np != 2) {
    if (me == 0) {
      printf("You have to use exactly 2 processes to run this program\n");
    }
    MPI_Finalize();	       /* Quit if there is only one processor */
    exit(0);
  }

  MPI_Buffer_attach(buff, size);       /* Attach a buffer */
  x = 12345; y = me;   /* Initialize */

  if (me == 0) {    /* Process 0 does this */
    
    printf("Process %d sending %d to process 1\n", me, x);
    MPI_Bsend(&x, 1, MPI_INT, 1, tag, MPI_COMM_WORLD);  /* Buffered send */
    printf("Process %d receiving from process 1\n", me);
    MPI_Recv (&y, 1, MPI_INT, 1, tag, MPI_COMM_WORLD, &status);
    printf ("Process %d received value %d\n", me, y);
    
  } else {         /* Process 1 does this */

    /*
      This program will not deadlock even though we change the order of
      the send and receive calls here, because the messages are
      buffered so the processes can continue the execution without
      waiting for the other process to receive the message
    */

    MPI_Recv (&y, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
    MPI_Bsend (&y, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);  /* Buffered send */ 
  }

  MPI_Buffer_detach(&buff, &size);  /* Detach the buffer */
  MPI_Finalize();
  exit(0);
}
