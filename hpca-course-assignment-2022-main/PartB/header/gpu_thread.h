// Create other necessary functions here


#include <stdio.h>
#include <cuda.h>
#include <stdlib.h>


__global__ void  matrixMul(const int *a, const int *b, int *c, int N){
  int row = blockIdx.y * blockDim.y + threadIdx.y;
  int coloumn = blockIdx.x * blockDim.x + threadIdx.x;
  int row_A=row;
  int coloumn_B=coloumn;


if (((row %2)==0) && (coloumn%2==0) && (row + 1 <N) && (coloumn+1<N))
{
 int tot=0;
    for(int i = 0; i < N; i++) 
     {
       tot = tot  + a[row_A * N + i] * b[i * N + coloumn_B];
       tot = tot  + a[(row_A+1) * N + i] * b[i * N + coloumn_B];
       tot = tot  + a[row_A * N + i] * b[i * N + (coloumn_B+1)];
       tot = tot  + a[(row_A+1) * N + i] * b[i * N + (coloumn_B+1)];
     }
     int row_C = row_A>>1;
     int coloumn_C = coloumn_B>>1;
     int index_C = row_C * (N>>1) + coloumn_C;
     c[index_C] = tot;
}
}


void gpuThread(int N, int *matA, int *matB, int *rslt)
{
  
  int *d_a, *d_b, *d_c;
  cudaMalloc((void**)&d_a, N*N*sizeof(int));
  cudaMalloc((void**)&d_b, N*N*sizeof(int));
  cudaMalloc((void**)&d_c, N/2*N/2*sizeof(int));

  
  cudaMemcpy(d_a, matA, N*N*sizeof(int), cudaMemcpyHostToDevice);
  cudaMemcpy(d_b, matB, N*N*sizeof(int), cudaMemcpyHostToDevice);

  
  int num_thread = 32;

  
  int num_block = N/num_threads ;

  
   dim3 threads(num_threads, num_threads);
   dim3 blocks(num_block, num_block);

  
  matrixMul<<<blocks, threads>>>(d_a,d_b,d_c,N);

  
  cudaMemcpy(rslt, d_c, N/2*N/2*sizeof(int),cudaMemcpyDeviceToHost);

  
  cudaFree(d_a);
  cudaFree(d_b);
  cudaFree(d_c);

}
