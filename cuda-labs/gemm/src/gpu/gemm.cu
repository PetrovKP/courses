extern "C" {
#include "gemm.h"
}
#include <cuda.h>
#include <cuda_runtime_api.h>

const int BLOCK_SIZE = 16;

__global__
void blas_sgemm_kernel(const int m, const int n, const int k, const float *a,
            const float *b, float *c) {
    int bx = blockIdx.x;
    int by = blockIdx.y;

    int tx = threadIdx.x;
    int ty = threadIdx.y;

    int ai = BLOCK_SIZE*n*by + n*ty;
    int bj = BLOCK_SIZE*bx + tx;

    float sum = 0.0f;
    for (int ki = 0; ki < n; ki++) {
        sum += a[ai + ki]*b[ki*k + bj];
    }

    c[by*k*BLOCK_SIZE + ty*k + bx*BLOCK_SIZE + tx] = sum;
}

__global__
void blas_sgemm_optimized_kernel(const int m, const int n, const int k, const float *a,
            const float *b, float *c) {

    const int row = threadIdx.x;
    const int col = threadIdx.y;
    const int globalRow = BLOCK_SIZE*blockIdx.x + row;
    const int globalCol = BLOCK_SIZE*blockIdx.y + col;

    __shared__ float as[BLOCK_SIZE][BLOCK_SIZE];
    __shared__ float bs[BLOCK_SIZE][BLOCK_SIZE];

    float acc = 0.0f;

    const int numTiles = k/BLOCK_SIZE;
    for (int t=0; t<numTiles; t++) {

        const int tiledRow = BLOCK_SIZE*t + row;
        const int tiledCol = BLOCK_SIZE*t + col;
        as[col][row] = a[globalRow*n + tiledCol];
        bs[col][row] = b[tiledRow*k + globalCol];

        __syncthreads();
        for (int i = 0; i < BLOCK_SIZE; i++) {
            acc += as[i][row]*bs[col][i];
        }
        __syncthreads();
    }

    c[globalRow*k + globalCol] = acc;
}


extern "C"
void cblas_sgemm_gpu_cuda(const int m, const int n, const int k, const float *a,
            const float *b, float *c, const mode_opt opt) {
    float *a_gpu, *b_gpu, *c_gpu;
    cudaMalloc((void**)&a_gpu, m*n*sizeof(float));
    cudaMalloc((void**)&b_gpu, n*k*sizeof(float));
    cudaMalloc((void**)&c_gpu, m*k*sizeof(float));

    cudaMemcpy(a_gpu, a, m*n*sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(b_gpu, b, n*k*sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(c_gpu, c, m*k*sizeof(float), cudaMemcpyHostToDevice);

    dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);
    dim3 dimGrid(m / dimBlock.x, k / dimBlock.y);
    if (opt == OPTIMIZED)
        blas_sgemm_optimized_kernel <<< dimGrid, dimBlock >>> (m, n, k, a_gpu, b_gpu, c_gpu);
    else
        blas_sgemm_kernel <<< dimGrid, dimBlock >>> (m, n, k, a_gpu, b_gpu, c_gpu);

    cudaMemcpy(c, c_gpu, m*k*sizeof(float), cudaMemcpyDeviceToHost);

    cudaFree(a_gpu);
    cudaFree(b_gpu);
    cudaFree(c_gpu);
}
