/* 
 * Compile:  g++ -std=c++11 -fopenmp -o exercise3 exercise3.cpp
 * Run:      ./exercise3 <number of threads>
 */

 /* 
 	a.	private : i,j, count, temp
		shared: a,n
	b.	No hay loop-carried dependency, las posiciones son independientes, no cambian en el tiempo
	c.  Si, haciendo saltos de sizeof(int) a la posición inicial de a

 */
#include <string.h>
#include <iostream>
#include <omp.h>   

using namespace std;

void count_sort(int a[], int n)
{
	int i,j, count;
	int * temp = new int[n];

	for(i=0 ; i<n ; ++i)
	{
		count = 0;
		for(j=0 ; j<n ; ++j)
		{
			if(a[j] < a[i])
				++count;
			else if(a[j] == a[i] && j<i)
				++count;
		}
		temp[count] = a[i];
	}

	memcpy(a, temp, n*sizeof(int));
	free(temp);
}


int main(int argc, char* argv[])
{
	int thread_count = strtol(argv[1], NULL, 10); 
	int n, i,j, count, my_rank;

	cout << "Number of elements : ";
	cin >> n;

	int * a = new int[n];
	for(size_t i=0 ; i<n ; cin >> a[i], ++i);
		

	int * temp = new int[n];

	#pragma omp parallel num_threads(thread_count) \
		default(none) \
		private(i, j, count, my_rank) \
		shared(a, n, temp)
		{
			my_rank = omp_get_thread_num();

			#pragma omp for schedule(dynamic, 1)
			for(i=0 ; i<n ; ++i)
			{
				count = 0;
				for(j=0 ; j<n ; ++j)
				{
					if(a[j] < a[i])
						++count;
					else if(a[j] == a[i] && j<i)
						++count;
				}
				temp[count] = a[i];
			}			
		}
	memcpy(a, temp, n*sizeof(int));


	for(size_t i=0 ; i<n ; cout << a[i] << " ", ++i);
	cout << endl;

	return 0; 
}
