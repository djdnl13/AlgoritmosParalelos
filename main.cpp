#include <iostream>
#include <time.h> 
#include <stdlib.h>

#define MAX 1000

typedef clock_t timer;

using namespace std;


int main()
{
	timer my_timer;
	double A[MAX][MAX], x[MAX], y[MAX];
	
	srand(time(NULL));
	/* Initialize A and x, assign y=0 */
	for( size_t i=0,j ; i<MAX ; ++i )
	{
		y[i] = 0;
		x[i] = (double)rand() / RAND_MAX;
		for( j=0 ; j<MAX ; ++j )
			A[i][j] = (double)rand() / RAND_MAX;
	}

	my_timer = clock();
	for( size_t i=0,j ; i<MAX ; ++i)
	{
		for( j=0 ; j<MAX ; ++j )
			y[i] += A[i][j] * x[j];
	}
	my_timer = clock() - my_timer;
	cout << "Time elapsed 1st loop : " << ((float)my_timer)/CLOCKS_PER_SEC << endl;

	/* Assign y = 0 */
	for( size_t i=0 ; i<MAX ; y[++i]=0 );

	my_timer = clock();
	for( size_t j=0,i ; j<MAX ; ++j)
	{
		for( i=0 ; i<MAX ; ++i )
			y[i] += A[i][j] * x[j];
	}
	my_timer = clock() - my_timer;
	cout << "Time elapsed 2st loop : " << ((float)my_timer)/CLOCKS_PER_SEC << endl;

	return 1;
}
