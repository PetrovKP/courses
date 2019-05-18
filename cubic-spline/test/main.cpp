//
// Created by kirill on 21.10.17.
//

#include <stdlib.h>

#include "spline.h"
#include "utils.h"
#include "math.h"

#define N 10

#define INIT_X(x)               \
for (int i = 0; i < N; i++)     \
  (x)[i] = i;                   \

#define INIT_Y(y)               \
for (int i = 0; i < N; i++)     \
  (y)[i] = sin(i);              \


void measure_float(void) {
  float *x        = (float *)malloc(sizeof(float)*N);
  float *y        = (float *)malloc(sizeof(float)*N);

  INIT_X(x);
  INIT_Y(y);

  MEASURE( CubicSpline<float> spline(N, x, y) );

  MEASURE( CubicSplineMkl<float> splineMkl(N, x, y) );

  printf("\n");

  for (float i = 0; i < N; i+=0.5) {
    printf("exp1 = %f res1 = %f\n", sin(i), spline.interpolate(i));
    printf("exp2 = %f res2 = %f\n", sin(i), splineMkl.interpolate(i));
//    TEST(interpolate(i, spline) == i*i);
  }

  free(x);
  free(y);
}


int main() {
  printf("Float\n\n");
  measure_float();

  return 0;
}