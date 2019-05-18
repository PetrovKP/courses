//
// Created by kirill on 21.10.17.
//

#include <stdlib.h>

#include "utils.h"
#include "axpy.h"

#define N (1<<27)

#define INIT_X(x)               \
for (int i = 0; i < N; i++)     \
  (x)[i] = i*0.3 + 3;           \

#define INIT_Y(y)               \
for (int i = 0; i < N; i++)     \
  (y)[i] = i*100 + 0.1;         \

#define INIT_EXPECTED(y)        \
for (int i = 0; i < N; i++) {   \
  (y)[i] = i*100 + 0.1;         \
  (y)[i] += i*0.3 + 3;          \
}                               \

void measure_float(void) {
  float *x        = (float *)malloc(sizeof(float)*N);
  float *y        = (float *)malloc(sizeof(float)*N);
  float *expected = (float *)malloc(sizeof(float)*N);

  INIT_X(x);
  INIT_Y(y);
  INIT_EXPECTED(expected);

  MEASURE(saxpy(N, 1, x, 1, y, 1, CPU));

  TEST(scompare_array(y, expected, N))
  INIT_Y(y);

  MEASURE(saxpy(N, 1, x, 1, y, 1, CPU_OPENMP));

  TEST(scompare_array(y, expected, N))
  INIT_Y(y);

  MEASURE(saxpy(N, 1, x, 1, y, 1, GPUCUDA));

  TEST(scompare_array(y, expected, N))
  INIT_Y(y);

  free(x);
  free(y);
  free(expected);
}

void measure_double(void) {
  double *x        = (double *)malloc(sizeof(double)*N);
  double *y        = (double *)malloc(sizeof(double)*N);
  double *expected = (double *)malloc(sizeof(double)*N);

  INIT_X(x);
  INIT_Y(y);
  INIT_EXPECTED(expected);

  MEASURE(daxpy(N, 1, x, 1, y, 1, CPU));

  TEST(dcompare_array(y, expected, N))
  INIT_Y(y);

  MEASURE(daxpy(N, 1, x, 1, y, 1, CPU_OPENMP));

  TEST(dcompare_array(y, expected, N))
  INIT_Y(y);

  MEASURE(daxpy(N, 1, x, 1, y, 1, GPUCUDA));

  TEST(dcompare_array(y, expected, N))
  INIT_Y(y);

  free(x);
  free(y);
  free(expected);
}

int main() {
  printf("Float\n\n");
  measure_float();

  printf("Double\n\n");
  measure_double();

  return 0;
}