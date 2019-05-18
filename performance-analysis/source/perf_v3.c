#include "utils.h"
#include <stdlib.h>
#include <immintrin.h>

#define ALIGNMENT 64

typedef struct {
    size_t len;
    FPType* data;
} my_vector;

size_t my_vector_len(my_vector *v) {
    return v->len;
}

__attribute__((optimize("unroll-loops")))
void Kernel(m_float* vec_res, const FPType* const data, const size_t n){
    for (size_t j = LENVEC; j < n; j += LENVEC) {
        m_float vec_in = mm_load(data + j);
        *vec_res = mm_mul(*vec_res, vec_in);
    }
}

void Multiplication(my_vector* v, FPType* res) {
    const FPType* const data = v->data;
    const size_t n = v->len;
    _mm_prefetch((char *)data, _MM_HINT_T0);

    m_float vec_res = mm_load(data);

    Kernel(&vec_res, data, n);

    *res = 1.0;
    FPType tmp[LENVEC];
    mm_store(tmp, vec_res);
    for (size_t k = 0u; k < LENVEC; k++) {
        #ifdef DEBUG
            printf("%lf \n", tmp[k]);
        #endif
        *res *= tmp[k];
    }
}

int main() {
    printf("Version 3:\n");

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
