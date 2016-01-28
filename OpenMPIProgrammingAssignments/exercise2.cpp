/* 
 * Compile:  g++ -std=c++11 -fopenmp -o exercise2 exercise2.cpp
 * Run:      ./exercise2 <number of threads>
 */
#include <iostream>
#include <random>
#include <functional>
#include <omp.h>   

 using namespace std;

 int main(int argc, char* argv[])
 {
 	int thread_count = strtol(argv[1], NULL, 10); 

 	double pi=0, x, y, distance_squared;
 	int i, number_of_tosses, number_in_circle=0, divisions;

 	default_random_engine generator;
 	uniform_real_distribution<double> distribution(-1,1);

 	cout << "Insert number of tosses : " ;
 	cin >> number_of_tosses;

 	divisions = number_of_tosses/thread_count;

	#  pragma omp parallel num_threads(thread_count)\
 	default(none) reduction(+:pi)\
 	private(number_in_circle, x, y, distance_squared, i, distribution, generator)\
 	shared(number_of_tosses, divisions, thread_count)
 	{
 		auto my_random = bind( distribution, generator );
 		for(i=0 ; i<number_of_tosses ; ++i)
 		{
 			x = my_random();
 			y = my_random();
 			distance_squared = x*x + y*y;
 			if(distance_squared <= 1)
 				++number_in_circle;
 		}	
 		pi = number_in_circle/((double)number_of_tosses);
 	}
 	
 	cout << "Estimated value of pi : " << pi << endl;

 	return 0; 
 }
