#include "utils.h"
#include <stdlib.h>

typedef struct {
    long len;
    FPType *data;
} my_vector;

long my_vector_len(my_vector *v) {
    return v->len;
}

void foo_bar(my_vector *v, FPType *res) {
    long i;
    *res = 1;
    for (i = 0; i < my_vector_len(v); i++) {
        *res *= v->data[i];
    }
}

int main() {
    printf("Version 0:\n");

    FPType res, expected;
    FPType* a = (FPType*)malloc(M*sizeof(FPType));
    my_vector v = {M, a};

    gen_random_vector(a);
    reference_calculate(a, &expected);

    MEASURE(foo_bar(&v, &res));

    test(expected, res);

    free(a);
    return 0;
}
