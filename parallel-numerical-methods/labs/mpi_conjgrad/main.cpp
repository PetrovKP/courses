//
// Created by kirill on 01.05.19.
//

#include <algorithm>
#include <iostream>

#include "config_threads.hpp"
#include "conjgrad.h"
#include "util.h"

int main(int argc, char* argv[]) {
  MPI_Init(&argc, &argv);

  const int max_iter = 512;
  const int sizes[] = {1024, 2048};
  const double eps = 1e-8;

  double global_time;

  int n_proc, rank_proc;
  MPI_Comm_size(MPI_COMM_WORLD, &n_proc);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank_proc);
  RandomGenerator random(563);

  for (int n : sizes) {
    double* A = new double[n * n];
    GeneratePositiveDefiniteMatrix(A, n, random);

    auto gen = random.GenRealVector(n, -2.0, 2.0);
    double* b = gen.data();
    double* x_seq = new double[n];
    double* x_mpi = new double[n];

    double time_sequentinal = 0.0;

    if (rank_proc == ROOT_ID) {
      // serial
      int count = 0;
      auto ts = GetTime();
      SLE_Solver_Sequentional(A, n, b, eps, max_iter, x_seq, count);
      auto tf = GetTime();
      time_sequentinal = GetMS(ts, tf);
      if (rank_proc == ROOT_ID)
        printf("Sequential: size %d;\t count %d;\t Time %8.2lf ms;\n", n, count,
               time_sequentinal);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    // mpi
    {
      int count = 0;
      auto ts = GetTime();
      SLE_Solver(A, n, b, eps, max_iter, x_mpi, count);
      auto tf = GetTime();
      double time_run = GetMS(ts, tf);
      MPI_Reduce(&time_run, &global_time, 1, MPI_DOUBLE, MPI_MAX, ROOT_ID,
                 MPI_COMM_WORLD);
      if (rank_proc == ROOT_ID)
        printf(
            "MPI: np(%d): size %d;\t count %d;\t Time %8.2lf ms;\t BOOST: "
            "%lf\n",
            n_proc, n, count, global_time, time_sequentinal / global_time);
    }
    // if (rank_proc == ROOT_ID) printf("Diff %lf\n", CheckDiff(x_mpi, x_seq,
    // n));

    delete[] x_mpi;
    delete[] x_seq;
  }
  MPI_Finalize();
  return 0;
}
