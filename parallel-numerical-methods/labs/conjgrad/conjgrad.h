//
// Created by kirill on 30.04.19.
//

#ifndef PARALLEL_ALGORITHMS_CONJGRAD_H
#define PARALLEL_ALGORITHMS_CONJGRAD_H

#include "config_threads.hpp"
#include <cstdlib>
#include <cmath>
#include <vector>

void MulMatVec(double* result, const double* const A, const int n, const double* const b)
{
    #ifdef USE_TBB
        tbb::parallel_for(tbb::blocked_range<int>(0, n), [&](const tbb::blocked_range<int>& r) {
            for (int i = r.begin(); i != r.end(); ++i)
    #else
        #pragma omp parallel for
        for (int i = 0; i < n; i++)
    #endif
    {
        result[i] = 0.0;
        for (int j = 0; j < n; j++)
        {
            result[i] += A[i*n + j] * b[j];
        }
    }
    #ifdef USE_TBB
        });
    #endif

}

void SLE_Solver(const double* const A, const int n, const double* const b,
    const double eps, const int max_iter, double* x, int& count)
{
    double* internalMemory = new double[n*5];

    double* r = internalMemory + n*0;
    double* n_r = internalMemory + n*1;

    double* p = internalMemory + n*2;
    double* n_p = internalMemory + n*3;

    double* Ap = internalMemory + n*4;

    std::fill_n(x, n, 1.0);

    MulMatVec(Ap, A, n, x);

    for(int i = 0; i < n; i++)
    {
        r[i] = p[i] = b[i] - Ap[i];
    }

    double norm_b = 0.0;

    for(int i = 0; i < n; i++)
    {
        norm_b += b[i] * b[i];
    }

    int iter = 0;
    for(; iter < max_iter; iter++)
    {
        MulMatVec(Ap, A, n, p);

        double numerator = 0.0;
        double denominator = 0.0;

        for(int i = 0; i < n; i++)
        {
            numerator += r[i] * r[i];
            denominator += p[i] * Ap[i];
        }

        const double alpha = numerator / denominator;

        double check = 0.0;

        for(int i = 0; i < n; i++)
        {
            const double alphaP = alpha * p[i];
            const double x_prev = x[i];
            x[i] += alphaP;
            check += (x[i] - x_prev) * (x[i] - x_prev);
        }

        if(check < eps*eps*norm_b)
        {
            break;
        }

        for(int i = 0; i < n; i++)
        {
            n_r[i] = r[i] - alpha * Ap[i];
        }

        numerator = 0.0;
        denominator = 0.0;
        for(int i = 0; i < n; i++)
        {
            numerator += n_r[i] * n_r[i];
            denominator += r[i] * r[i];
        }

        const double beta = numerator / denominator;

        for(int i = 0; i < n; i++)
        {
            n_p[i] = n_r[i] + beta * p[i];
        }

        std::swap(r, n_r);
        std::swap(p, n_p);
    }

    count = iter;

    delete[] internalMemory;
}


#endif //PARALLEL_ALGORITHMS_CONJGRAD_H
