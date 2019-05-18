//// Copyright 2018 Kirill Petrov

#include <omp.h>
#include <stdlib.h>
#include <math.h>

#define FLOAT_TYPE
// #define DEBUG

#ifdef DEBUG
    #define M 64
#else
    #define M (1<<28)
#endif

#define GETT_TIME() omp_get_wtime()

typedef struct {
  double start;
  double finish;
} t_timer;

static t_timer timer;

#define MEASURE(function)                                                                      \
    timer.start = GETT_TIME();                                                                 \
    function;                                                                                  \
    timer.finish = GETT_TIME();                                                                \
    printf("Performance time = %.4f msec \n", (double)((timer.finish - timer.start)*1000.0));  \

#if defined(FLOAT_TYPE)

#define ABS(x) fabsf(x)
#define EPS 1e-4

#define LENVEC 8
#define DGREE_LENVEC 3
#define m_float __m256
#define m_int   __m256i

#define mm_set1(val)               _mm256_set1_ps(val)
#define mm_load(vec)               _mm256_load_ps(vec)
#define mm_mul(a,b)                _mm256_mul_ps(a,b)
#define mm_store(out, in)          _mm256_storeu_ps(out, in)
#define mm_stream(out, in)         _mm256_stream_ps(out, in)
typedef float FPType;

#elif defined(DOUBLE_TYPE)

#define MPI_TYPE MPI_DOUBLE
#define ABS(x) fabs(x)
#define EPS 1e-8

#define LENVEC 4
#define DGREE_LENVEC 2
#define m_float __m256d
#define m_int   __m128i

#define mm_set1(vec)              _mm256_set1_pd(vec)
#define mm_load(vec)              _mm256_loadu_pd(vec)
#define mm_mul(a,b)               _mm256_mul_pd(a,b)
#define mm_store(out, in)         _mm256_storeu_pd(out, in)
#define _mm256_stream_ps(out, in)         _mm256_stream_ps(out, in)

typedef double FPType;

#endif

void gen_random_vector(FPType* vector) {
    for (size_t i = 0u; i < M; i++) {
        vector[i] = (FPType)(rand()%8) / 10 + 1;
        // vector[i] = (FPType)(rand()%8) / 10 + 1;
        // vector[i] = (FPType)i+1;
        // vector[i] = (FPType)1.1;
    }
}

void reference_calculate(const FPType* const vector, FPType* res) {
    *res = 1.0;
    for (size_t i = 0u; i < M; i++) {
        *res *= vector[i];
    }
}

void test(FPType reference, FPType result) {
    if (ABS(reference - result)/reference < EPS) {
      printf("-------------\n");
      printf("---SUCCESS---\n");
      printf("-------------\n");
        printf("expected = %.3lf; result = %.3lf\n", reference, result);

    } else {
      printf("-------------\n");
      printf("---FAILED!---\n");
      printf("expected = %.3lf; result = %.3lf\n", reference, result);
      printf("-------------\n");
    }
    printf("\n");
}
