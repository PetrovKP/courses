extern "C" {
#include "axpy.h"
}
#include <cuda.h>
#include <cuda_runtime_api.h>

const int block_size = 256;

__global__
void saxpy_kernel(int n, float a, float *x, int incx, float *y, int incy) {
  int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i*incx < n && i*incy < n)
        y[i*incy] += x[i*incx]*a;
}

__global__
void daxpy_kernel(int n, double a, double *x, int incx, double *y, int incy) {
  int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i*incx < n && i*incy < n)
        y[i*incy] += x[i*incx]*a;
}

extern "C"
void saxpy_gpu(int n, float a, float *x, int incx, float *y, int incy) {
    float *x_gpu, *y_gpu;
    cudaMalloc((void**)&x_gpu, n*sizeof(float));
    cudaMalloc((void**)&y_gpu, n*sizeof(float));

    cudaMemcpy(x_gpu, x, n*sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(y_gpu, y, n*sizeof(float), cudaMemcpyHostToDevice);

    int num_blocks = (n + block_size - 1)/block_size;

    saxpy_kernel <<< num_blocks, block_size >>> (n,a,x_gpu,incx,y_gpu,incy);

    cudaMemcpy(y, y_gpu, n*sizeof(float), cudaMemcpyDeviceToHost);

    cudaFree(x);
    cudaFree(y);
}

extern "C"
void daxpy_gpu(int n, double a, double *x, int incx, double *y, int incy) {
    double *x_gpu, *y_gpu;
    cudaMalloc((void**)&x_gpu, n*sizeof(double));
    cudaMalloc((void**)&y_gpu, n*sizeof(double));

    cudaMemcpy(x_gpu, x, n*sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(y_gpu, y, n*sizeof(double), cudaMemcpyHostToDevice);

    int num_blocks = (n + block_size - 1)/block_size;

    daxpy_kernel <<< num_blocks, block_size >>> (n,a,x_gpu,incx,y_gpu,incy);

    cudaMemcpy(y, y_gpu, n*sizeof(double), cudaMemcpyDeviceToHost);

    cudaFree(x);
    cudaFree(y);
}
