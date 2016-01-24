#include <iostream>
#include <mpi.h>

using namespace std;

int main(int argc, char * argv[])
{
	int comm_size, current_rank;
	int * my_vector = NULL, local_total;
	int divisions, length;

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &current_rank);

	if(current_rank == 0)
	{		
		cout << "Insert length : " ;
		cin >> length;

		my_vector = new int[length];
		divisions = length/comm_size;

		for(size_t i=0 ; i<length ; ++i)
		{
			cout << "Insert number at pos " << i << " : ";
			cin >> my_vector[i];
		}
	}
	
	MPI_Bcast(&length, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&divisions, 1, MPI_INT, 0, MPI_COMM_WORLD);

	if(my_vector == NULL)
		my_vector = new int[length];

	MPI_Bcast(my_vector, length, MPI_INT, 0, MPI_COMM_WORLD);

	local_total = 0;
	for(size_t i=current_rank*divisions ; i<((current_rank+1)*divisions) ; ++i)
		local_total += my_vector[i];

	if(current_rank&1)
		MPI_Send(&local_total, 1, MPI_INT, current_rank-1, 0, MPI_COMM_WORLD); 
	else
	{
		int odd_total;

		MPI_Recv(&odd_total, 1, MPI_INT, current_rank+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		local_total += odd_total;

		for(size_t i=2,j; i<comm_size; i<<=1)
		{
			for(j=i; j<comm_size; j+= i<<1)
			{
				if(current_rank == j)
					MPI_Send(&local_total, 1, MPI_INT, j-i, 0, MPI_COMM_WORLD);
				else if(current_rank == j-i)
				{			
					MPI_Recv(&odd_total, 1, MPI_INT, j, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					local_total += odd_total;
				}
			}
		}
	}

	if(current_rank == 0)
	{				
		cout << "Total : " << local_total << endl;
	}
	MPI_Finalize();

	return 0;
}