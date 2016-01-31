#include <iostream>

using namespace std;

	// B
__global__ void kerner_matrix_addition(float** A, float** B, float** C, int n)
{
	int i = threadIdx.x;
	int j = threadIdx.y;

	if(i<n and j<n)
		A[i][j] = B[i][j] + C[i][j];
}

	// C
__global__ void kerner_matrix_addition_row(float** A, float** B, float** C, int n)
{
	int i = threadIdx.x;

	for(int j=0 ; j<n ; ++j)
		A[i][j] = B[i][j] + C[i][j];
}

	// D
__global__ void kerner_matrix_addition_column(float** A, float** B, float** C, int n)
{
	int j = threadIdx.y;

	for(int i=0 ; i<n ; ++i)
		A[i][j] = B[i][j] + C[i][j];
}

void vecAdd(float ** A, float ** B, float ** C, int n)
{
	int size = n*n*sizeof(float);
	float **d_A, **d_B, **d_C;		

	cudaMalloc((void **) &d_B, size);
	cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice);

	cudaMalloc((void **) &d_C, size);
	cudaMemcpy(d_B, B, size, cudaMemcpyHostToDevice);

	cudaMalloc((void **) &d_A, size);

	//vecAddKernel<<<ceil(n/256.0), 256>>>(d_A, d_B, d_C, n);
	//dim3 threadsPerBlock(n, n);

	// Kernel invocation code
	kerner_matrix_addition<<<(1,1), (n,n)>>>(d_A, d_B, d_C, n);
	
	/*kerner_matrix_addition_row<<<1, (n,1)>>>(d_A, d_B, d_C, n);
	kerner_matrix_addition_column<<<1, (1,n)>>>(d_A, d_B, d_C, n);
	*/

	cudaMemcpy(A, d_A, size, cudaMemcpyDeviceToHost);

 		// Free device memory for A, B, C
	cudaFree(d_A); cudaFree(d_B); cudaFree (d_C);
}

int main()
{
	float ** A, ** B, ** C;
	int n = 256;
	
	A = new float * [n];
	B = new float * [n];
	C = new float * [n];

	for(size_t i=0 ; i<n ; i++)
	{
		A[i] = new float[n];
		B[i] = new float[n];
		C[i] = new float[n];

		for(size_t j=0 ; j<n ; j++)
		{
			A[i][j] = 1;
			B[i][j] = 2;
			C[i][j] = 0;
		}
	}

	vecAdd(A, B, C, n);

	for(size_t i=0 ; i<n ; i++)
	{
		for(size_t j=0 ; j<n ; j++)
		{
			cout << A[i][j] << " ";
		}
		cout << endl;		
	}



	return 0;
}