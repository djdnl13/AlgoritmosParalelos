#include <iostream>

typedef int t_number;

using namespace std;

__global__ void Matrix_Vector_Multiplication(t_number * dev_a , t_number * dev_b , t_number * dev_c, size_t n)
{
	size_t tid = blockIdx.x*blockDim.x + threadIdx.x;

	if(tid < n)
	{
		dev_a[tid] = 0;
		for(size_t i=0 ; i<n ; ++i)
			dev_a[tid] += dev_b[n*i+tid] * dev_c[i];
	}
}

int main()
{
	size_t n = 10;
	size_t size_vector = n;
	size_t size_matrix = n*n;
	t_number * host_a, * host_b, * host_c;
	t_number * dev_a, * dev_b, * dev_c;

	
	host_a = new t_number[size_vector];
	host_b = new t_number[size_matrix];
	host_c = new t_number[size_vector];
	
	for(size_t i=0; i<size_vector ; ++i)
		host_c[i] = 1 ;

	for(size_t i=0 ; i<size_matrix ; ++i)
		host_b[i] = 1;

	cudaMalloc((void **)&dev_a , size_vector*sizeof(t_number));
	cudaMalloc((void **)&dev_b , size_matrix*sizeof(t_number));
	cudaMalloc((void **)&dev_c , size_vector*sizeof(t_number));

	cudaMemcpy(dev_b, host_b, size_matrix*sizeof(t_number), cudaMemcpyHostToDevice);
	cudaMemcpy(dev_c, host_c, size_vector*sizeof(t_number), cudaMemcpyHostToDevice);
		
	Matrix_Vector_Multiplication<<<1, n>>>(dev_a, dev_b, dev_c, n);

	cudaMemcpy(host_a, dev_a, size_vector*sizeof(t_number), cudaMemcpyDeviceToHost);

	for(size_t i=0; i<size_vector ; ++i)
		cout << host_a[i] << " ";
	cout << endl;
	cudaFree(dev_a);
	cudaFree(dev_b);
	cudaFree(dev_c);
	return 0;
}
