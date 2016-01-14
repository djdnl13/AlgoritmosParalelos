#include <iostream>
#include <time.h>
#include <stdlib.h>

#define MAX 1000
#define LOOPS 100

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
        x[i] = (double)rand() / RAND_MAX;
        for( j=0 ; j<MAX ; ++j )
            A[i][j] = (double)rand() / RAND_MAX;
    }

    float time_loop_1, time_loop_2;
    time_loop_1 = time_loop_2 = 10;
    for( size_t h=0 ; h<LOOPS ; ++h )
    {
        /* Assign y = 0 */
        for( size_t i=0 ; i<MAX ; y[++i]=0 );

		/* Lopp 1 */
        my_timer = clock();
        for( size_t i=0,j ; i<MAX ; ++i)
        {
            for( j=0 ; j<MAX ; ++j )
                y[i] += A[i][j] * x[j];
        }
        my_timer = clock() - my_timer;

        if(((float)my_timer)/CLOCKS_PER_SEC < time_loop_1)
            time_loop_1 = ((float)my_timer)/CLOCKS_PER_SEC;

        /* Assign y = 0 */
        for( size_t i=0 ; i<MAX ; y[++i]=0 );

		/* Loop 2 */
        my_timer = clock();
        for( size_t j=0,i ; j<MAX ; ++j)
        {
            for( i=0 ; i<MAX ; ++i )
                y[i] += A[i][j] * x[j];
        }
        my_timer = clock() - my_timer;

        if(((float)my_timer)/CLOCKS_PER_SEC < time_loop_2)
            time_loop_2 = ((float)my_timer)/CLOCKS_PER_SEC;
    }

    cout << "Time elapsed 1st loop : " << time_loop_1 << endl;
    cout << "Time elapsed 2st loop : " << time_loop_2 << endl;

    return 1;
}

