//
// Created by kirill on 01.05.19.
//

#include <mpi.h>

#include <algorithm>
#define CATCH_CONFIG_RUNNER
#include <catch.hpp>
#include <ctime>
#include <functional>
#include <iostream>
#include <limits>
#include <set>
#include <string>

#include "config_threads.hpp"
#include "conjgrad.h"
#include "util.h"

int main(int argc, char* argv[]) {
  MPI_Init(&argc, &argv);
  int result = Catch::Session().run(argc, argv);
  MPI_Finalize();
  return result;
}

bool IsEqual(const double* const a, const double* const b, const size_t n,
             const double eps = 1e-3) {
  bool res = true;
  for (size_t i = 0; i < n; i++) {
    if (std::abs(a[i] - b[i]) > eps) {
      printf("%lu %lf %lf %lf\n", i, a[i], b[i], std::abs(a[i] - b[i]));
      res = false;
      break;
    }
  }
  return res;
}

TEST_CASE("Simple") {
  GetInfo();
  {
    SetNThreads(1);
    const int n = 3;
    const double A[] = {25, 15, -5, 15, 18, 0, -5, 0, 11};

    double b[] = {35, 33, 6};
    double expectedX[] = {1, 1, 1};
    double x[3] = {0.0};

    int count = 0;

    SLE_Solver(A, n, b, 0.001, 100, x, count);

    REQUIRE(IsEqual(x, expectedX, 3, 1e-7));
  }
  {
    SetNThreads(1);
    const int n = 4;
    const double A[] = {
        45, 46, 28, 32, 46, 66, 43, 53, 28, 43, 30, 35, 32, 53, 35, 45,
    };

    double b[] = {151, 208, 136, 165};
    double expectedX[] = {1, 1, 1, 1};
    double x[n] = {0.0};

    int count = 0;

    SLE_Solver(A, n, b, 0.001, 100, x, count);

    REQUIRE(IsEqual(x, expectedX, n, 1e-7));
  }
}

TEST_CASE("Thead save simple") {
  const int n = 4;
  const double A[] = {
      45, 46, 28, 32, 46, 66, 43, 53, 28, 43, 30, 35, 32, 53, 35, 45,
  };

  double b[] = {43, 12, 64, 32};
  int count = 0;

  double x1[n] = {0.0};
  double x2[n] = {0.0};
  double x4[n] = {0.0};
  {
    SetNThreads(1);
    SLE_Solver(A, n, b, 0.001, 100, x1, count);
  }
  {
    SetNThreads(2);
    SLE_Solver(A, n, b, 0.001, 100, x2, count);
  }
  REQUIRE(IsEqual(x1, x2, n, 1e-13));
  {
    SetNThreads(4);
    SLE_Solver(A, n, b, 0.001, 100, x4, count);
  }
  REQUIRE(IsEqual(x1, x4, n, 1e-13));
}

TEST_CASE("Middle sizes") {
  {
    SetNThreads(1);
    const int n = 12;

    RandomGenerator random(123);

    double* A = new double[n * n];
    GeneratePositiveDefiniteMatrix(A, n, random);

    auto x_vec = random.GenRealVector(n, -5.0, 5.0);
    const double* expected_x = x_vec.data();

    double* b = new double[n];
    double* x = new double[n];

    ReferenceMulMatVec(b, A, n, expected_x);

    int count = 0;
    SLE_Solver(A, n, b, 1e-8, 1e5, x, count);

    REQUIRE(IsEqual(x, expected_x, n, 1e-3));
    delete[] A;
    delete[] b;
    delete[] x;
  }
  {
    SetNThreads(4);
    const int n = 12;

    RandomGenerator random(123);

    double* A = new double[n * n];
    GeneratePositiveDefiniteMatrix(A, n, random);

    auto x_vec = random.GenRealVector(n, -5.0, 5.0);
    const double* expected_x = x_vec.data();

    double* b = new double[n];
    double* x = new double[n];

    ReferenceMulMatVec(b, A, n, expected_x);

    int count = 0;
    SLE_Solver(A, n, b, 1e-8, 1e5, x, count);

    REQUIRE(IsEqual(x, expected_x, n, 1e-3));
    delete[] A;
    delete[] b;
    delete[] x;
  }
}

TEST_CASE("Big size") {
  {
    SetNThreads(1);
    const int n = 512;

    RandomGenerator random(123);

    double* A = new double[n * n];
    GeneratePositiveDefiniteMatrix(A, n, random);

    auto x_vec = random.GenRealVector(n, -2.0, 2.0);
    const double* expected_x = x_vec.data();

    double* b = new double[n];
    double* x = new double[n];

    ReferenceMulMatVec(b, A, n, expected_x);

    int count = 0;
    SLE_Solver(A, n, b, 0, 1e3, x, count);

    REQUIRE(IsEqual(x, expected_x, n, 1e-1));
    delete[] A;
    delete[] b;
    delete[] x;
  }
  {
    SetNThreads(1);
    const int n = 1024;

    RandomGenerator random(123);

    double* A = new double[n * n];
    GeneratePositiveDefiniteMatrix(A, n, random);

    auto x_vec = random.GenRealVector(n, -2.0, 2.0);
    const double* expected_x = x_vec.data();

    double* b = new double[n];
    double* x = new double[n];

    ReferenceMulMatVec(b, A, n, expected_x);

    int count = 0;
    SLE_Solver(A, n, b, 0, 1e3, x, count);

    REQUIRE(IsEqual(x, expected_x, n, 1e-1));
    delete[] A;
    delete[] b;
    delete[] x;
  }
}
