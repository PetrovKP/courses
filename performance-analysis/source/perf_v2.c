#include "utils.h"
#include <stdlib.h>
#include <immintrin.h>

#define ALIGNMENT 64
#ifdef DEBUG
    #define DGREE_BUCKET 5
    #define BUCKET_SIZE (1<<DGREE_BUCKET)
#else
    #define DGREE_BUCKET 15
    #define BUCKET_SIZE (1<<DGREE_BUCKET)
#endif

#define LAMBDA 4

typedef struct {
    size_t len;
    FPType* data;
} my_vector;

size_t my_vector_len(my_vector *v) {
    return v->len;
}

void Multiplication(my_vector* v, FPType* res) {
    FPType local_res = 1;
    FPType* data = v->data;
    size_t n = v->len;
    size_t count_backet = n >> DGREE_BUCKET;

    #ifdef DEBUG
        printf("count_backet %lu\n", count_backet);
    #endif
    for (size_t i = 0u; i < count_backet; i++) {
        const size_t stride = i*BUCKET_SIZE;

        for (size_t remainder = BUCKET_SIZE >> 1; remainder > LENVEC*LAMBDA; remainder = remainder >> 1) {
            #ifdef DEBUG
                printf("remainder %lu\n", remainder);
            #endif
            for (size_t j = 0u; j < remainder >> DGREE_LENVEC; j++) {
                // Основное мясо
                m_float vec_a1 = mm_load(data + (2*j + 0)*LENVEC + stride);
                m_float vec_b1 = mm_load(data + (2*j + 1)*LENVEC + stride);

                m_float vec_c1 = mm_mul(vec_a1, vec_b1);

                mm_store(&data[(j + 0)*LENVEC + stride], vec_c1);

                #ifdef DEBUG
                    printf("data[%d] %lf\n", j*LENVEC + stride, data[j*LENVEC + stride]);
                #endif
                // ----
            }
        }
        // _mm_prefetch((char *)&data[stride], _MM_HINT_T0);
        for (size_t k = 0u; k < LENVEC*LAMBDA; k++) {
            local_res *= data[stride + k];
        }
    }

    *res = local_res;
}

int main() {
    printf("Version 2:\n");

    FPType res, expected;
    FPType* a = (FPType*)aligned_alloc(ALIGNMENT, M*sizeof(FPType));
    my_vector v = {M, a};

    gen_random_vector(a);
    reference_calculate(a, &expected);

    MEASURE(Multiplication(&v, &res));

    test(expected, res);

    free(a);
    return 0;
}
