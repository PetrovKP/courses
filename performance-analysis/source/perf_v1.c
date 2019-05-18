#include "utils.h"
#include <stdlib.h>

#define ALIGNMENT 64

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
    size_t n = my_vector_len(v);

    size_t median = n/2;
    while(median != 4) {
        for (size_t i = 0u; i < median; i++) {
            data[i] *= data[i + median];

        }

        median = median/2;
    }

    for (size_t i = 0u; i < median*2; i++) {
        local_res *= data[i];
    }

    *res = local_res;
}

int main() {
    printf("Version 1:\n");

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
