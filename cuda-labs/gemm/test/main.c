//
// Created by kirill on 21.10.17.
//

#include <stdlib.h>

#include "utils.h"
#include "gemm.h"

#define M (1<<11)
#define N (1<<11)
#define K (1<<11)

#define INIT_A(a)                 \
for (int i = 0; i < M; i++)       \
  for (int j = 0; j < N; j++)     \
    (a)[i*N + j] = 0.42;          \

#define INIT_B(b)                 \
for (int i = 0; i < N; i++)       \
  for (int j = 0; j < K; j++)     \
    (b)[i*K + j] = 0.27;          \

void measure_float(void) {
  float *a = (float *)malloc(sizeof(float)*M*N);
  float *b = (float *)malloc(sizeof(float)*N*K);
  float *c = (float *)malloc(sizeof(float)*M*K);
  float *cRef = (float *)malloc(sizeof(float)*M*K);

  INIT_A(a);
  INIT_B(b);

  MEASURE(cblas_sgemm(M, N, K, a, b, cRef, CPU));

  MEASURE(cblas_sgemm(M, N, K, a, b, c, GPUCUDA));
  TEST(scompare_array(c, cRef, M*K));

  MEASURE(cblas_sgemm(M, N, K, a, b, c, GPUCUDA_OPTIMAZE));
  TEST(scompare_array(c, cRef, M*K));

  MEASURE(cblas_sgemm(M, N, K, a, b, c, GPUCL));
  TEST(scompare_array(c, cRef, M*K));

  MEASURE(cblas_sgemm(M, N, K, a, b, c, GPUCL_OPTIMAZE));
  TEST(scompare_array(c, cRef, M*K));

  free(a);
  free(b);
  free(c);
}

int main() {
  printf("Float\n\n");
  measure_float();

  return 0;
}