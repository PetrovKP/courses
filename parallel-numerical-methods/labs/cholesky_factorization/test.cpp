//
// Created by kirill on 27.10.18.
//

#include <catch.hpp>

#include "cholesky.h"
#include "util.h"

#include <iostream>
#include <algorithm>
#include <string>
#include <set>
#include <functional>
#include <util.h>
#include <limits>
#include <ctime>

void Validate(const double* const A, const double * const L, int n, double thresh)
{
    double* LLT = new double[n*n];
    Syrk(LLT, L, n, Uplo::lower);
    for (int i = 0 ; i < n; i++)
    {
        for(int j = 0; j < n; j ++)
        {
            const double rdiff = std::fabs((LLT[i*n + j] - A[i*n + j])/A[i*n + j]);
            REQUIRE(rdiff <= thresh);
            if (rdiff > thresh)
            {
                printf("(%d,%d):LLT(i,j):%f,A(i,j):%f (delta: %1.15f)\n",
                       i,j,LLT[i*n + j], A[i*n + j], rdiff);

            }
        }
    }
    delete[] LLT;
}

TEST_CASE("Simple")
{
    {
        int n = 3;
        double A[] =
        {
            25,  15,  -5,
            15,  18,   0,
            -5,   0,   11
        };
        double L[n*n] = {0};
        Cholesky_Decomposition(A, L, n);
        REQUIRE(L[0*n + 0] == 5);
        REQUIRE(L[1*n + 0] == 3);
        REQUIRE(L[1*n + 1] == 3);
        REQUIRE(L[2*n + 0] == -1);
        REQUIRE(L[2*n + 1] == 1);
        REQUIRE(L[2*n + 2] == 3);
    }
    {
        int n = 4;

        double A[] =
        {
            18,  22,   54,   42,
            22,  70,   86,   62,
            54,  86,  174,  134,
            42,  62,  134,  106
        };

        double L[n*n]= {0};

        Cholesky_Decomposition(A, L, n);

        Validate(A, L, n, 1e-5);
    }
}

TEST_CASE("Middle")
{
    {
        int n = 20;
        double * A = new double[n*n];
        double * L = new double[n*n];
        RandomGenerator random(3121);
        GeneratePositiveDefiniteMatrix(A, n, random);
        std::fill_n(L, n*n, 0);

        Cholesky_Decomposition(A, L, n);

        Validate(A, L, n, 1e-5);
    }
}
