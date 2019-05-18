
#include "bicg.h"

#include <iostream>
#include <algorithm>
#include <omp.h>
#include "util.h"

static inline void InitCrsMatrix(CRSMatrix &crs_matrix, int size, int notNull)
{
    crs_matrix.n = crs_matrix.m = size;
    crs_matrix.nz = notNull;
    crs_matrix.val.resize(notNull);
    crs_matrix.colIndex.resize(notNull);
    crs_matrix.rowPtr.resize(size + 1);
}

static inline void GenerateCrsMatrix(CRSMatrix &crs_matrix, int size, int count_nz_in_row, RandomGenerator& random)
{
    int f;
    InitCrsMatrix(crs_matrix, size, count_nz_in_row * size);

    for (int i = 0; i < crs_matrix.n; i++)
    {
        for (int j = 0; j < count_nz_in_row; j++)
        {
            do
            {
                crs_matrix.colIndex[i * count_nz_in_row + j] = random.GenInt(0, crs_matrix.n);
                f = 0;
                for (int k = 0; k < j; k++)
                {
                    if (crs_matrix.colIndex[i * count_nz_in_row + j] ==
                        crs_matrix.colIndex[i * count_nz_in_row + k])
                        f = 1;
                }
            } while (f == 1);
        }

        for (int j = 0; j < count_nz_in_row - 1; j++)
        {
            for (int k = 0; k < count_nz_in_row - 1; k++)
            {
                if (crs_matrix.colIndex[i * count_nz_in_row + k] >
                    crs_matrix.colIndex[i * count_nz_in_row + k + 1])
                {
                    std::swap(crs_matrix.colIndex[i * count_nz_in_row + k],
                        crs_matrix.colIndex[i * count_nz_in_row + k + 1]);
                }
            }
        }
    }

    for (int i = 0; i < count_nz_in_row * crs_matrix.n; i++)
    {
        crs_matrix.val[i] = random.GenFloat(-2.0, 2.0);
        int c = 0;
        for (int j = 0; j <= crs_matrix.n; j++)
        {
            crs_matrix.rowPtr[j] = c;
            c += count_nz_in_row;
        }
    }
}

int main(int argc, char* argv[])
{
    const int max_iter = 1000;
    int threads[] = {1, 2, 4};
    int sizes[] = {200, 400, 800, 1000};

    RandomGenerator random(563);

    for (int size : sizes)
    {

        CRSMatrix A;
        GenerateCrsMatrix(A, size, int(size*0.5), random);

        auto gen = random.GenRealVector(A.n, -2.0, 2.0);
        double* b = gen.data();
        double* x = new double[A.n];

        for (int thread: threads)
        {
            omp_set_num_threads(thread);
            int count = 0;
            {
                double ts = omp_get_wtime();
                SLE_Solver_CRS_BICG(A, b, 1e-4, max_iter, x, count);
                double tf = omp_get_wtime();
                printf("size %d\t nz %d\t count %d \t nThread %d\t Time %f ms\n", A.n, A.nz, count, thread, (tf - ts)*1000);
            }
        }

        delete[] x;
    }
    return 0;
}


