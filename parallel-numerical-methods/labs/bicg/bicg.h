//
// Created by kirill on 16.03.19.
//

#ifndef PARALLEL_ALGORITHMS_BICG_H
#define PARALLEL_ALGORITHMS_BICG_H

#include <cstdlib>
#include <cmath>
#include <omp.h>
#include <vector>

struct CRSMatrix
{
    int n;
    int m;
    int nz;
    std::vector<double> val;
    std::vector<int> colIndex;
    std::vector<int> rowPtr;
};

void Gemm(double *result, const double * const A, int n, int offset)
{
    # pragma omp parallel for schedule(dynamic)
    for(int i = 0; i < n; i++) {
        for(int j = i; j < n; j++) {
            double sum = 0.0;
            for(int k = 0; k < offset; k++){
                sum += A[i*offset + k] * A[j*offset + k];
            }
            result[i*n + j] = sum;
            result[j*n + i] = sum;
        }
    }
}

void Gemv(double* result, const CRSMatrix& A, const double* const b)
{
    #pragma omp parallel for
    for(int i = 0; i < A.n; i++) {
        result[i] = 0.;
        for(int j = A.rowPtr[i]; j < A.rowPtr[i + 1]; j++) {
            result[i] += A.val[j] * b[A.colIndex[j]];
        }
    }
}

void Dot(double* result, const double* const a, const double* const b, const int n)
{
    double sum = 0.0;
    #pragma omp parallel for reduction(+:sum)
    for(int i = 0; i < n; i++) {
        sum += a[i] * b[i];
    }
    *result = sum;
}

void Transpose(const CRSMatrix& A, CRSMatrix& At)
{
    At.n = A.m;
    At.m = A.n;
    At.nz = A.nz;
    At.rowPtr.resize(A.n + 1);
    At.colIndex.resize(A.nz);
    At.val.resize(A.nz);

    std::fill(At.rowPtr.begin(), At.rowPtr.end(), 0);
    for(int i = 0; i < A.nz; i++)
    {
        At.rowPtr[A.colIndex[i] + 1]++;
    }
    int accumulationRows = 0;
    for(int i = 1; i <= A.n; i++)
    {
        const int cur = At.rowPtr[i];
        At.rowPtr[i] = accumulationRows;
        accumulationRows += cur;
    }

    for(int i = 0; i < A.n; i++)
    {
        for(int j = A.rowPtr[i]; j < A.rowPtr[i + 1]; j++)
        {
            int& ind = At.rowPtr[A.colIndex[j] + 1];
            At.val[ind] = A.val[j];
            At.colIndex[ind] = i;
            ind++;
        }
    }
}

void SLE_Solver_CRS_BICG(CRSMatrix& A, double* b, double eps, int max_iter, double* x, int& count)
{
    CRSMatrix At;
    Transpose(A, At);

    const int n = A.n;

    double* internalMemory = new double[n*10];

    double* r = internalMemory + n*0;
    double* bi_r = internalMemory + n*1;
    double* n_r = internalMemory + n*2;
    double* nbi_r = internalMemory + n*3;

    double* p = internalMemory + n*4;
    double* bi_p = internalMemory + n*5;
    double* n_p = internalMemory + n*6;
    double* nbi_p = internalMemory + n*7;

    double* Ap = internalMemory + n*8;
    double* Atbip = internalMemory + n*9;

    std::fill_n(x, n, 1.0);

    Gemv(Ap, A, x);

    #pragma omp parallel for schedule(static)
    for(int i = 0; i < n; i++)
    {
        r[i] = bi_r[i] = p[i] = bi_p[i] = b[i] - Ap[i];
    }

    int iter = 0;
    for(; iter < max_iter; iter++)
    {
        Gemv(Ap, A, p);
        Gemv(Atbip, At, bi_p);

        double numerator = 0.0;
        double denominator = 0.0;
        #pragma omp parallel for reduction(+:numerator,denominator)
        for(int i = 0; i < n; i++)
        {
            numerator += bi_r[i] * r[i];
            denominator += bi_p[i] * Ap[i];
        }

        const double alpha = numerator / denominator;

        #pragma omp parallel for schedule(static)
        for(int i = 0; i < n; i++)
        {
            n_r[i] = r[i] - alpha * Ap[i];
            nbi_r[i] = bi_r[i] - alpha * Atbip[i];
        }

        double check = 0.0;
        #pragma omp parallel for reduction(+:check)
        for(int i = 0; i < n; i++) {
            const double alphaP = alpha * p[i];
            x[i] += alphaP;
            check += alphaP * alphaP;
        }

        if(check < eps*eps)
        {
            break;
        }

        denominator = numerator;
        check = 0.0;
        numerator = 0.0;
        #pragma omp parallel for reduction(+:numerator, check)
        for(int i = 0; i < n; i++)
        {
            numerator += nbi_r[i] * n_r[i];
            check += nbi_r[i] * nbi_r[i];
        }

        if(check < eps*eps)
        {
            break;
        }
        const double beta = numerator / denominator;

        #pragma omp parallel for schedule(static)
        for(int i = 0; i < n; i++)
        {
            n_p[i] = n_r[i] + beta * p[i];
            nbi_p[i] = nbi_r[i] + beta * bi_p[i];
        }

        std::swap(r, n_r);
        std::swap(p, n_p);
        std::swap(bi_r, nbi_r);
        std::swap(bi_p, nbi_p);
    }

    count = iter;

    delete[] internalMemory;
}


#endif //PARALLEL_ALGORITHMS_BICG_H
