	__global__ void kerner_matrix_addition(float * A, float ** B, float* C, int n)
	{
		int i = threadIdx.x;

		if(i>=n)
			return;

		A[i] = 0;
		for(size_t j=0; j<n ; ++j)
			A[i] += B[i][j] * C[i];
	}

	void vecAdd(float * A, float ** B, float * C, int n)
	{
		int size_vector = n*sizeof(float);
		int size_matrix = n*n*sizeof(float);
		float **d_A, **d_B, **d_C;

		cudaMalloc((void **) &d_B, size_matrix);
		cudaMemcpy(d_A, A, size_matrix, cudaMemcpyHostToDevice);

		cudaMalloc((void **) &B_C, size_vector);
		cudaMemcpy(d_B, B, size_vector, cudaMemcpyHostToDevice);

		cudaMalloc((void **) &d_A, size_vector);
		
		//vecAddKernel<<<ceil(n/256.0), 256>>>(d_A, d_B, d_C, n);		
		kerner_matrix_addition<<<1, n>>>(d_A, d_B, d_C, n, m);		

		cudaMemcpy(A, d_A, size_vector, cudaMemcpyDeviceToHost);

 		// Free device memory for A, B, C
		cudaFree(d_A); cudaFree(d_B); cudaFree (d_C);
	}