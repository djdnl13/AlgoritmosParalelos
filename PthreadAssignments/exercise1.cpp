#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> 
#include <string.h>
#include <iostream>
#include <semaphore.h>

using namespace std;

const int MAX_THREADS = 64;

/* Global variable:  accessible to all threads */
int thread_count;  

void Usage(char* prog_name);
void *Hello(void* rank);  /* Thread function */
void print_histogram(int *bin_counts, double min, int n_bins, double width); 
int * compute_elems_count(int elem_total, int proc_count);
int find_bin(double v, double min, double width, int n_bins);
int * count_elems_per_bin(double *vector, int start, int end, int n_bins, double min, double width);
void find_max_min(double *vector, int length, double *max, double *min);
void  * thread_function(void * args_p);
/*--------------------------------------------------------------------*/


double max1, min1, *vector, width;
int *bin_counts, *bin_counts_aux, comm_sz,
      elem_count,*elems_per_process, first_elem,
      i, j, my_rank, n_bins, length;

sem_t my_sem;

int main(int argc, char* argv[]) {
   long       thread;  /* Use long in case of a 64-bit system */
   pthread_t* thread_handles; 

   /* Get number of threads from command line */
   if (argc != 2) Usage(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);  
   if (thread_count <= 0 || thread_count > MAX_THREADS) Usage(argv[0]);

   thread_handles = new pthread_t[thread_count]; 

   scanf("%d %d", &length, &n_bins); 
   vector = new double[length];
   bin_counts_aux = new int[n_bins];

   memset(bin_counts_aux, 0, sizeof(int)*n_bins);
   for(i = 0; i < length; i++)
      scanf("%lf", &vector[i]);
    
   find_max_min(vector, length, &max1, &min1);
   width = (max1 - min1)/n_bins;
   elems_per_process = compute_elems_count(length, thread_count);
   first_elem = 0;

   
   for (thread = 0; thread < thread_count; thread++)  
	{ 
      first_elem += elems_per_process[i-1];
      pthread_create(&thread_handles[thread], NULL, thread_function, (void*) first_elem);  
	}
	
   for (thread = 0; thread < thread_count; thread++) 
      pthread_join(thread_handles[thread], NULL); 

   print_histogram(bin_counts, min1, n_bins, width);

   free(thread_handles);
   return 0;
}  /* main */

void * thread_function(void * args_p)
{
    long first_elem = (long) args_p;
	double * vector2 = new double[length];	
	bin_counts_aux = count_elems_per_bin(vector2, first_elem, first_elem + elem_count, n_bins, min1, width);
	return NULL;
}

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

void *Hello(void* rank) {
   long my_rank = (long) rank;  /* Use long in case of 64-bit system */ 

   printf("Hello from thread %ld of %d\n", my_rank, thread_count);

   return NULL;
}  /* Hello */

/*-------------------------------------------------------------------*/
void Usage(char* prog_name) {
   fprintf(stderr, "usage: %s <number of threads>\n", prog_name);
   fprintf(stderr, "0 < number of threads <= %d\n", MAX_THREADS);
   exit(0);
}  /* Usage */
