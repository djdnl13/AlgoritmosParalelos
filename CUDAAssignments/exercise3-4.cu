/*
Part 1 of the function allocates space in the device (GPU) memory to hold
copies of the A, B, and C vectors, and copies the vectors from the host
memory to the device memory.
*/
	int size = n*sizeof(float);
	float *d_A, *d_B, *d_C;

	cudaMalloc((void **) &d_B, size);
	cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice);

	cudaMalloc((void **) &d_C, size);
	cudaMemcpy(d_B, B, size, cudaMemcpyHostToDevice);

	cudaMalloc((void **) &d_A, size);
/*
Part 2 launches parallel execution of the
actual vector addition kernel on the device. 
*/
	vecAddKernel<<<ceil(n/256.0), 256>>>(d_A, d_B, d_C, n);