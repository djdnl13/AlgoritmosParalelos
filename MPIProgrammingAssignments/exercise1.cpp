#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <iostream>

using namespace std;

void print_histogram(int *bin_counts, double min, int n_bins, double width) { 
  for(size_t i=0,j ; i<n_bins ; ++i){
    cout << endl <<(min + i*width) << " - " <<  (min + (i + 1)*width) << " : ";
    for(j=0; j<bin_counts[i]; cout << "*", ++j);
  }
} 

int * compute_elems_count(int elem_total, int proc_count) {
   int *elems, module;
   elems = new int[proc_count];

   for(size_t i=0; i<proc_count; ++i){
    elems[i] = elem_total/proc_count;
    module = elem_total%proc_count;

    if(module != 0)
      if(module - (i+1) >= 0)  
        elems[i]++;
  }

  return elems;
} 

int find_bin(double v, double min, double width, int n_bins){
  if( (v >= (min)) && (v <= (min + width)))
    return 0;

  for(size_t i = 1; i < n_bins; ++i)
    if( (v > (min + (i - 1)*width)) && (v <= (min + (i + 1)*width)))
      return i;

  return -1;
}

int * count_elems_per_bin(double *vector, int start, int end, int n_bins, double min, double width) {

  int bin, *bin_counts;
  bin_counts = new int[n_bins];
  memset(bin_counts, 0, sizeof(int)*n_bins);

  for(size_t i=start; i<end ; ++i) {
    bin = find_bin(vector[i], min, width, n_bins);
    bin_counts[bin]++;
  }

  return bin_counts;
} 
     

void find_max_min(double *vector, int length, double *max, double *min) {
  *max = *min = vector[0];
  
  for(size_t i = 1; i < length; ++i) {
    if(*min > vector[i])
      *min = vector[i];
    if (*max < vector[i])
      *max = vector[i];
  }
} 

int main() {
  double max, min, *vector, width;
  int *bin_counts, *bin_counts_aux, comm_sz,
      elem_count,*elems_per_process, first_elem,
      i, j, my_rank, n_bins, length;


  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  if(my_rank == 0) {
    scanf("%d %d", &length, &n_bins); 
    vector = new double[length];
    bin_counts_aux = new int[n_bins];

    memset(bin_counts_aux, 0, sizeof(int)*n_bins);
    for(i = 0; i < length; i++)
       scanf("%lf", &vector[i]);
    
    find_max_min(vector, length, &max, &min);
    width = (max - min)/n_bins;
    elems_per_process = compute_elems_count(length, comm_sz);
    first_elem = 0;

    for(i = 1; i < comm_sz; ++i) {
      first_elem += elems_per_process[i - 1];

      MPI_Send(&length, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
      MPI_Send(&n_bins, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
      MPI_Send(&min, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
      MPI_Send(&width, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
      MPI_Send(vector, length, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
      MPI_Send(&first_elem, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
      MPI_Send(&elems_per_process[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD);
    }

    bin_counts = count_elems_per_bin(vector, 0, elems_per_process[0], n_bins,min, width);

    for(i = 1; i < comm_sz; ++i){
      MPI_Recv(bin_counts_aux, n_bins, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      for(j = 0; j < n_bins; ++j){
        bin_counts[j] += bin_counts_aux[j];
      }
    }

    print_histogram(bin_counts, min, n_bins, width);
    free(bin_counts);
  } else {
    MPI_Recv(&length, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&n_bins, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&min, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&width, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    vector = new double[length];

    MPI_Recv(vector, length, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&first_elem, n_bins, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&elem_count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    bin_counts_aux = count_elems_per_bin(vector, first_elem, 
                                         first_elem + elem_count, n_bins,
                                         min, width);

    MPI_Send(bin_counts_aux, n_bins, MPI_INT, 0, 0, MPI_COMM_WORLD);
  }

  free(vector); free(bin_counts_aux);

  MPI_Finalize();

  return 0;
}
