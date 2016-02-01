#include <iostream>

typedef int t_number;

using namespace std;

__global__ void Matrix_Addition(t_number * dev_a , t_number * dev_b , t_number * dev_c, size_t n)
{
     size_t tid = blockIdx.x*blockDim.x + threadIdx.x;

     if (tid < n*n)
         dev_c[tid] = dev_a[tid] + dev_b[tid];
}

__global__ void Matrix_Addition_Row(t_number * dev_a , t_number * dev_b , t_number * dev_c, size_t n)
{
    size_t tid = blockIdx.x*blockDim.x + threadIdx.x;
    for(size_t i=0 ; i<n ; ++i)
         dev_c[tid+i] = dev_a[tid+i] + dev_b[tid+i];
}

__global__ void Matrix_Addition_Column(t_number * dev_a , t_number * dev_b , t_number * dev_c, size_t n)
{
    size_t tid = blockIdx.x*blockDim.x + threadIdx.x;
    for(size_t i=0 ; i<n ; ++i)
         dev_c[tid+i*n] = dev_a[tid+i*n] + dev_b[tid+i*n];
}

int main()
{
      t_number * Host_a, * Host_b, * Host_c;
      t_number * dev_a , * dev_b, * dev_c ;
      size_t n = 10;
      t_number size = n*n;

      cudaMalloc((void **)&dev_a , size*sizeof(t_number));
      cudaMalloc((void **)&dev_b , size*sizeof(t_number));
      cudaMalloc((void **)&dev_c , size*sizeof(t_number));

      Host_a = new t_number[size];
      Host_b = new t_number[size];
      Host_c = new t_number[size];

      for (size_t i = 0; i<size ; ++i)
      {
            Host_a[i] = i ;
            Host_b[i] = i*2 ;
      }

      cudaMemcpy(dev_a , Host_a , size*sizeof(t_number) , cudaMemcpyHostToDevice);
      cudaMemcpy(dev_b , Host_b , size*sizeof(t_number) , cudaMemcpyHostToDevice);

//      Matrix_Addition<<<N, N>>>(dev_a , dev_b , dev_c, n);
//      Matrix_Addition_Row<<<n, 1>>>(dev_a , dev_b , dev_c, n);
      Matrix_Addition_Column<<<1, n>>>(dev_a , dev_b , dev_c, n);

      cudaMemcpy(Host_c , dev_c , size*sizeof(t_number) , cudaMemcpyDeviceToHost);

      for(size_t i=0 ; i<size ; ++i)
      {
          if(i%n == 0)
             cout << endl;
          cout << Host_c[i] << " " ;          
      }
      cout << endl;

      cudaFree(dev_a) ;
      cudaFree(dev_b) ;
      cudaFree(dev_c) ;

      return 0 ;
}
