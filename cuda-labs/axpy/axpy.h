//
// Created by kirill on 21.10.17.
//

#ifndef CUDA_LABS_AXPY_H
#define CUDA_LABS_AXPY_H

#include <stdio.h>
#include <utils.h>

void saxpy(int n, float a, float *x, int incx, float *y, int incy, mode device);
void daxpy(int n, double a, double *x, int incx, double *y, int incy, mode device);

void saxpy_gpu(int n, float a, float *x, int incx, float *y, int incy);
void daxpy_gpu(int n, double a, double *x, int incx, double *y, int incy);
void saxpy_omp(int n, float a, float *x, int incx, float *y, int incy);
void daxpy_omp(int n, double a, double *x, int incx, double *y, int incy);
void saxpy_ref(int n, float a, float *x, int incx, float *y, int incy);
void daxpy_ref(int n, double a, double *x, int incx, double *y, int incy);

#endif //CUDA_LABS_AXPY_H
