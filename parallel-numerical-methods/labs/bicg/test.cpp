//
// Created by kirill on 27.10.18.
//

#include <catch.hpp>

#include "bicg.h"
#include "util.h"

#include <iostream>
#include <algorithm>
#include <string>
#include <set>
#include <functional>
#include <util.h>
#include <limits>
#include <ctime>

bool IsEqual(const double * const a, const double * const b, const size_t n, const double eps = 1e-3)
{
    bool res = true;
    for (size_t i = 0; i < n; i++)
    {
        if (std::abs(a[i] - b[i]) > eps)
        {
            res = false;
            break;
        }
    }
    return res;
}

TEST_CASE("Simple")
{
    {
        /*
        2, 1, 1
        1, -1, 0,
        3, -1, 2
        */

        std::vector<double> val = { 2, 1,  1, 1, -1, 3, -1, 2 };
        std::vector<int> colIndex = { 0, 1, 2, 0, 1, 0, 1, 2 };
        std::vector<int> rowPtr = {0, 3, 5, 8};

        CRSMatrix matrix = {};
        matrix.n = 3;
        matrix.m = 3;
        matrix.nz = 8;
        matrix.val = val;
        matrix.rowPtr = rowPtr;
        matrix.colIndex = colIndex;

        double b[] = { 2, -2, 2 };
        double expectedX[] = { -1, 1, 3 };

        double * x = new double[3];

        int count = 0;

        SLE_Solver_CRS_BICG(matrix, b, 0.001, 100, x, count);

        REQUIRE(IsEqual(x, expectedX, 3, 0.001));
    }
}

