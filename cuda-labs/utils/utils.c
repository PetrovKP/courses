//
// Created by kirill on 28.10.17.
//

#include <utils.h>

#include <math.h>
#include <float.h>
#include <memory.h>

int dcompare_array(double const *x, double const *y, size_t n) {
  for (size_t i = 0; i < n; i++)
    if (fabs(x[i] - y[i]) >  DBL_EPSILON*1e3 )
      return 0;
  return 1;
}

int scompare_array(float const *x, float const *y, size_t n) {
  for (size_t i = 0; i < n; i++)
    if (fabs(x[i] - y[i]) >  1e-2 )
      return 0;
  return 1;
}

void reset_array(void *arr) {
  memset(arr, 0, sizeof(arr));
}
void fprint_array(float *arr, size_t n) {
  for (int i = 0; i < n; i++) {
    printf("%.3f ",arr[i]);
  }
  printf("\n");
}

void fprint_matrix(float *arr, size_t m, size_t n) {
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      printf("%.3f\t", arr[i*n + j]);
    }
    printf("\n");
  }
  printf("\n");
}

