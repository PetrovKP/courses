//
// Created by kirill on 21.10.17.
//

#include "axpy.h"


void saxpy(int n, float a, float *x, int incx, float *y, int incy, mode device) {
  if (device == GPUCUDA) {
    #ifdef GPU_CUDA
      printf("run to GPU\n");
      saxpy_gpu(n, a, x, incx, y, incy);
    #endif
  } else if (device == CPU_OPENMP) {
    saxpy_omp(n, a, x, incx, y, incy);
    printf("run to OpenMP\n");
  } else if (device == CPU) {
    printf("run\n");
    saxpy_ref(n, a, x, incx, y, incy);
  }
}

void saxpy_omp(int n, float a, float *x, int incx, float *y, int incy) {
  #pragma omp parallel for
  for (int i = 0; i < n; i++) {
    if (i*incx < n && i*incy < n)
      y[i*incy] += x[i*incx]*a;
  }
}

void saxpy_ref(int n, float a, float *x, int incx, float *y, int incy) {
  for (int i = 0; i < n; i++) {
    if (i*incx < n && i*incy < n)
      y[i*incy] += x[i*incx]*a;
  }
}

void daxpy(int n, double a, double *x, int incx, double *y, int incy, mode device) {
  if (device == GPUCUDA) {
    #ifdef GPUCUDA
        printf("run to GPU\n");
        daxpy_gpu(n, a, x, incx, y, incy);
    #endif
  } else if (device == CPU_OPENMP) {
    printf("run to OpenMP\n");
    daxpy_omp(n, a, x, incx, y, incy);
  } else if (device == CPU) {
    printf("run\n");
    daxpy_ref(n, a, x, incx, y, incy);
  }
}

void daxpy_omp(int n, double a, double *x, int incx, double *y, int incy) {
  #pragma omp parallel for
  for (int i = 0; i < n; i++) {
    if (i*incx < n && i*incy < n)
      y[i*incy] += x[i*incx]*a;
  }
}

void daxpy_ref(int n, double a, double *x, int incx, double *y, int incy) {
  for (int i = 0; i < n; i++) {
    if (i*incx < n && i*incy < n)
      y[i*incy] += x[i*incx]*a;
  }
}
