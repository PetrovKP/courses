//
// Created by kirill on 01.05.19.
//

#include <algorithm>
#include <iostream>

#include "config_threads.hpp"
#include "conjgrad.h"
#include "util.h"

int main(int argc, char* argv[]) {
  GetInfo();

  const int max_iter = 1024;
  const int threads[] = {2, 3, 4};
  const int sizes[] = {512, 1024};
  const double eps = 1e-8;

  RandomGenerator random(563);

  for (int n : sizes) {
    double* A = new double[n * n];
    GeneratePositiveDefiniteMatrix(A, n, random);

    auto gen = random.GenRealVector(n, -2.0, 2.0);
    double* b = gen.data();
    double* x = new double[n];

    double time_sequentinal = 0.0;

    {
      // serial
      SetNThreads(1);
      int count = 0;
      auto ts = GetTime();
      SLE_Solver(A, n, b, eps, max_iter, x, count);
      auto tf = GetTime();
      time_sequentinal = GetMS(ts, tf);
      printf("Sequential: size %d;\t count %d;\t Time %lf ms;\n", n, count,
             time_sequentinal);
    }

    for (int thread : threads) {
      SetNThreads(thread);
      int count = 0;
      auto ts = GetTime();
      SLE_Solver(A, n, b, eps, max_iter, x, count);
      auto tf = GetTime();
      double time_run = GetMS(ts, tf);
      printf("size %d;\t count %d;\t nThread %d;\t Time %lf ms;\t BOOST: %lf\n",
             n, count, thread, time_run, time_sequentinal / time_run);
    }

    delete[] x;
  }
  return 0;
}
