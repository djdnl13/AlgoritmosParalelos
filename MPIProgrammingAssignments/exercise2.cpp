#include <iostream>
#include <random>
#include <functional>
#include <mpi.h>

using namespace std;


int main(int argc, char * argv[])
{
	double global_pi_estimate, local_pi_estimate, x, y, distance_squared;
	long number_of_tosses, number_in_circle, toss, divisions;
	int comm_size, current_rank;

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &current_rank);

	if(current_rank == 0)
	{		
		cout << "Insert number of tosses : " ;
		cin >> number_of_tosses;

		divisions = number_of_tosses/comm_size;		
	}
	MPI_Bcast(&divisions, 1, MPI_LONG, 0, MPI_COMM_WORLD);
	MPI_Bcast(&number_of_tosses, 1, MPI_LONG, 0, MPI_COMM_WORLD);
	
	MPI_Barrier(MPI_COMM_WORLD);

	default_random_engine generator;
	uniform_real_distribution<double> distribution(-1,1);
	auto my_random = bind( distribution, generator );
	
	local_pi_estimate = 0;
	number_in_circle = 0;	
	MPI_Barrier(MPI_COMM_WORLD);
	for(toss=0 ; toss<divisions ; ++toss)
	{
		x = my_random();
		y = my_random();
		distance_squared = x*x + y*y;
		if(distance_squared <= 1)
			++number_in_circle;
	}
	local_pi_estimate = 4*number_in_circle/(double) number_of_tosses;

	MPI_Reduce(&local_pi_estimate, &global_pi_estimate, 1, MPI_DOUBLE, 
		MPI_SUM, 0, MPI_COMM_WORLD);
		
	if(current_rank == 0)
	{				
		cout << "Estimated value of pi : " << global_pi_estimate << endl;
	}
	MPI_Finalize();

	return 0;
}