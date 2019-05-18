
#include "cholesky.h"

#include <iostream>
#include <algorithm>
#include <omp.h>
#include "util.h"

int main(int argc, char* argv[])
{
    int threads[] = {1, 2, 4};
    int sizes[] = {1000, 1200, 1400};

    RandomGenerator random(3121);

    for (int n : sizes)
    {
        for (int thread : threads)
        {
            omp_set_num_threads(thread);
            double * A = new double[n*n];
            double * L = new double[n*n];
            GeneratePositiveDefiniteMatrix(A, n, random);
            std::fill_n(L, n*n, 0);

            double ts = omp_get_wtime();
            Cholesky_Decomposition(A, L, n);
            double tf = omp_get_wtime();
            printf("Size %d\t nThread %d\t Time %f ms\n", n, thread, (tf - ts)*1000);
            delete[] A;
            delete[] L;
        }
    }
}


