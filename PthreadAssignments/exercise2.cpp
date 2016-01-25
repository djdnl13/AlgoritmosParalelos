#include <iostream>
#include <random>
#include <functional>
#include <pthread.h>

using namespace std;

int thread_count;
long divisions, number_of_tosses;
double global_pi_estimate=0;

pthread_mutex_t my_mutex;

void * calculate(void * arg)
{
	default_random_engine generator;
	uniform_real_distribution<double> distribution(-1,1);
	auto my_random = bind( distribution, generator );
	
	double number_in_circle=0, distance_squared, x, y;

	for(long toss=0 ; toss<divisions ; ++toss)
	{
		x = my_random();
		y = my_random();
		distance_squared = x*x + y*y;
		if(distance_squared <= 1)
			++number_in_circle;
	}
	
	pthread_mutex_lock(&my_mutex);		
	global_pi_estimate += 4*number_in_circle/(double) number_of_tosses;
	pthread_mutex_unlock(&my_mutex);		
}

int main(int argc, char * argv[])
{
	long thread;
	pthread_t * thread_handles;
	
	pthread_mutex_init(&my_mutex, NULL);

	cout << "Insert number of threads : " ;
	cin >> thread_count;
	thread_handles = new pthread_t[thread_count];

	cout << "Insert number of tosses : " ;
	cin >> number_of_tosses;

	divisions = number_of_tosses/thread_count;

	for(size_t i=0 ; i<thread_count ; ++i)
		pthread_create(&thread_handles[i], NULL, calculate, (void*) i);
	
	for(size_t i=0 ; i<thread_count ; ++i)
		pthread_join(thread_handles[i], NULL);

	cout << "Aprox. value of PI : " <<  global_pi_estimate << endl;

	return 0;
}
