/* 
 * Compile:  g++ -std=c++11 -fopenmp -o exercise4 exercise4.cpp
 * Run:      ./exercise4 <number of threads>
 */

 /* 
 	a.	No se puede paralelizar, hay loop-carried dependecy
 	b.  Si se puede paralelizar, sección crítica
 	c.  Si se puede paralelizar.
 	d.  Si se puede paralelizar.
 */
#include <string.h>
#include <iostream>
#include <omp.h>   

using namespace std;

#define FOR2

int main(int argc, char* argv[])
{
	int thread_count = strtol(argv[1], NULL, 10); 
	int n;

	cout << "Dimension of matrix : ";
	cin >> n;

	int ** matrix = new int * [n];
	int * asignments = new int[n];
	int * results = new int[n];
	cout << "Insert elements of matrix : ";
	for(size_t i=0,j ; i<n ; ++i)
	{
		matrix[i] = new int [n];
		for(j=0 ; j<n ; ++j)		
			cin >> matrix[i][j];
	}

	cout << "Insert asignments : ";
	for(size_t i=0 ; i<n ; cin>>asignments[i], ++i);

	int row, col;
	
	#ifdef FOR1
	for(row=n-1 ; row>=0 ; --row)
	{
		results[row] = asignments[row];
		int r;
		#pragma omp parallel num_threads(thread_count)
		#pragma omp for
		for(col=row+1 ; col<n ; ++col)
		{
			r = matrix[row][col]*results[col];		
			#pragma omp critical
				results[row] -= r;
		}
		results[row] /= matrix[row][row];
	}
	#endif

	#ifdef FOR2
		#pragma omp parallel num_threads(thread_count)
		#pragma omp for
		for(row=0 ; row<n ; ++row)
			results[row] = asignments[row];


		#pragma omp parallel num_threads(thread_count)
		#pragma omp for	
		for(col=n-1 ; col>= 0 ; --col)
		{
			//#pragma omp single
				results[col] /= matrix[col][col];
			//#pragma omp for
			for(row=0 ; row<col ; ++row)
				results[row] -= matrix[row][col]*results[col];
		}
	#endif

	for(size_t i=0 ; i<n ; cout << results[i], ++i);
	cout << endl;
	
	return 0; 
}
