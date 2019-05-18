//
// Created by kirill on 21.10.17.
//

#ifndef CUDA_LABS_GEMM_H
#define CUDA_LABS_GEMM_H

#include <stdio.h>
#include <utils.h>

typedef enum 
{
	OPTIMIZED,
	DEFAULT
} mode_opt;

void cblas_sgemm(const int m, const int n, const int k, const float *a, const float *b,
                 float *c, mode device);

void cblas_sgemm_gpu_cuda(const int m, const int n, const int k, const float *a, const float *b,
                 float *c, const mode_opt opt);

void cblas_sgemm_gpu_cl(const int m, const int n, const int k, const float *a, const float *b,
                 float *c, const mode_opt opt);

void cblas_sgemm_cpu(const int m, const int n, const int k, const float *a, const float *b,
                 float *c);

#endif //CUDA_LABS_GEMM_H
