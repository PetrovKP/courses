//
// Created by kirill on 26.02.19.
//

#ifndef PARALLEL_ALGORITHMS_CHOLESKY_H
#define PARALLEL_ALGORITHMS_CHOLESKY_H

#include <cstdlib>
#include <cmath>
#include <omp.h>

void Gemm(double *result, const double * const A, int n, int offset)
{
    # pragma omp parallel for schedule(dynamic)
    for(int i = 0; i < n; i++)
    {
        for(int j = i; j < n; j++)
        {
            double sum = 0.0;
            for(int k = 0; k < offset; k++)
            {
                sum += A[i*offset + k] * A[j*offset + k];
            }
            result[i*n + j] = sum;
            result[j*n + i] = sum;
        }
    }
}

void Cholesky(double *L, const double * const A, int n, int offset){
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < (i + 1); j++)
        {
            double temp = 0.0;
            for (int k = 0; k < j; k++)
            {
                temp += L[i*n + k] * L[j*n + k];
            }
            if (i == j)
            {
                L[i*n + j] = std::sqrt(A[i*offset + i] - temp);
            }
            else
            {
                L[i*n + j] = (A[i*offset + j] - temp) / L[j*n + j];
            }
        }
    }
}

void Cholesky_Decomposition(double *A, double *L, int n)
{
    const int size_blocks = std::min(20, n);

    double *L11 = new double[size_blocks * size_blocks];
    double *L21L21T = new double[n * n];
    double *L21 = new double[n * size_blocks];

    const int count_iterations = n / size_blocks;

    for (int it = 0; it < count_iterations; it++)
    {

        Cholesky(L11, A + n * size_blocks * it + size_blocks * it, size_blocks, n);

        for (int j = 0; j < size_blocks; j++)
        {
            for (int i = 0; i < n - size_blocks * (it + 1); i++)
            {
                double temp = A[(i + (it + 1) * size_blocks) * n + (j + it * size_blocks)];
                for (int k = 0; k <= j - 1; k++)
                {
                    temp -= L11[j*size_blocks + k] * L21[i*size_blocks + k];
                }
                L21[i*size_blocks + j] = temp / L11[j*size_blocks + j];
            }
        }

        Gemm(L21L21T, L21, (n - size_blocks * (it + 1)), size_blocks);

        # pragma omp parallel for schedule(static) collapse(2)
        for (int i = 0; i < n - size_blocks*(it + 1); i++)
        {
            for (int j = 0; j < n-size_blocks*(it + 1); j++)
            {
                A[(i + size_blocks*(it + 1))*n + (j + size_blocks*(it + 1))] -= L21L21T[i*(n-size_blocks*(it + 1)) +j];
            }
        }

        double *L11_ = L + n * size_blocks * it + size_blocks * it;
        double *L21_ = L11_ + n * size_blocks;
        # pragma omp parallel for schedule(dynamic)
        for (int i = 0; i < (n - size_blocks * (it + 1)); i++)
        {
            for (int j = 0; j < size_blocks; j++)
            {
                L21_[i*n + j] = L21[i*size_blocks + j];
            }
        }

        # pragma omp parallel for schedule(static)
        for (int i = 0; i < size_blocks; i++)
        {
            for (int j = 0; j <= i; j++)
            {
                L11_[i*n + j] = L11[i*size_blocks + j];
            }
        }
    }

    delete[] L21L21T;
    delete[] L11;
    delete[] L21;
}


#endif //PARALLEL_ALGORITHMS_CHOLESKY_H
