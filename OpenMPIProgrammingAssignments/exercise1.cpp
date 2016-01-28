/* 
 * Compile:  g++ -std=c++11 -fopenmp -o exercise1 exercise1.cpp
 * Run:      ./exercise1 <number of threads>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <omp.h>

using namespace std;

void Gen_bins(double min, double max, double bin_maxes[], int n_binss[], int n_bins)
{
   double bin_width;
   int   i;

   bin_width = (max - min)/n_bins;
   for (i = 0; i < n_bins; i++) {
      bin_maxes[i] = min + (i+1)*bin_width;
      n_binss[i] = 0;
   }
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

void print_histogram(int *bin_counts, double min, int n_bins, double width) { 
  for(size_t i=0,j ; i<n_bins ; ++i){
    cout << endl <<(min + i*width) << " - " <<  (min + (i + 1)*width) << " : ";
    for(j=0; j<bin_counts[i]; cout << "*", ++j);
  }
   cout << endl;
} 

int main(int argc, char* argv[]) {
   int n_bins, i, bin;
   double min, max, width;
   double* bin_maxes;
   int* n_binss;
   int length;
   double* vector;
   int thread_count = strtol(argv[1], NULL, 10); 

   cin >> length >> n_bins;

   bin_maxes = new double[n_bins];
   n_binss = new int[n_bins];
   vector = new double[length];

   for(i = 0; i < length; cin >>vector[i], i++);

   find_max_min(vector, length, &max, &min);
   width = (max - min)/n_bins;

   Gen_bins(min, max, bin_maxes, n_binss, n_bins);
   memset(n_binss, 0, sizeof(int)*n_bins);

   #pragma omp parallel num_threads(thread_count) \
      default(none) \
      private(i, bin) \
      shared(length, n_bins, n_binss,vector, min, max, width)
      #pragma omp for
      for (i = 0; i < length; ++i)
      {
         bin = (vector[i]-min)/width;
         if(bin == n_bins) --bin;
         #pragma omp critical(t)
         ++n_binss[bin];
      }

   print_histogram(n_binss, min, n_bins, width);

   delete vector, bin_maxes,n_binss;
   return 0;

}