//
// Created by kirill on 28.10.17.
//

#ifndef CUDA_LABS_UTILS_H
#define CUDA_LABS_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <printf.h>
#include <time.h>

typedef enum {
  GPU,
  GPU_OPTIMIZE,
  CPU_OPENMP,
  CPU
}  mode;

typedef struct {
  clock_t start;
  clock_t finish;
} t_timer;

t_timer timer;

int dcompare_array(double const *x, double const *y, size_t n);
int scompare_array(float const *x, float const *y, size_t n);

void reset_array(void * arr);

void fprint_array(float *arr, size_t n);

void fprint_matrix(float *arr, size_t m, size_t n);


#define TEST(b)              \
if (b) {                     \
  printf("-------------\n"); \
  printf("---SUCCESS---\n"); \
  printf("-------------\n"); \
} else {                     \
  printf("-------------\n"); \
  printf("---FAILED!---\n"); \
  printf("-------------\n"); \
}                            \
printf("\n");                \

#define MEASURE(function)                                                            \
  timer.start = clock();                                                             \
  function;                                                                          \
  timer.finish = clock();                                                            \
  printf("time = %.4f sec\n", (float)(timer.finish - timer.start)/CLOCKS_PER_SEC);   \

#define ERROR_MESSAGE(message) {                                              \
  printf("ERROR: %s in file: %s in line: %s\n", message, __FILE__, __LINE__); \
  return;                                                                     \
}                                                                            cppyb \

#ifdef __cplusplus
}
#endif

#endif //CUDA_LABS_UTILS_H
