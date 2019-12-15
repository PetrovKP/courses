//
// Created by kirill on 30.04.19.
//

#ifndef PARALLEL_ALGORITHMS_CONJGRAD_H
#define PARALLEL_ALGORITHMS_CONJGRAD_H

#include <mpi.h>

#include <cmath>
#include <cstdlib>
#include <vector>

#include "config_threads.hpp"

#define ROOT_ID 0

void MulMatVec(double* result, const double* const A, const int n, const int p,
               const double* const b) {
  // #pragma omp parallel for
  for (int i = 0; i < n; i++) {
    result[i] = 0.0;
    for (int j = 0; j < p; j++) {
      result[i] += A[i * n + j] * b[j];
    }
  }
}

void SLE_Solver(const double* const A, const int n, const double* const b,
                const double eps, const int max_iter, double* x, int& count) {
  int rank_proc, size_proc;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank_proc);
  MPI_Comm_size(MPI_COMM_WORLD, &size_proc);
  size_t local_n = n / size_proc;
  double* local_A = new double[local_n * n];

  MPI_Scatter(A, local_n * n, MPI_DOUBLE, local_A, local_n * n, MPI_DOUBLE,
              ROOT_ID, MPI_COMM_WORLD);

  double* internalMemoryLocal = new double[local_n * 4];
  double* local_r = internalMemoryLocal + local_n * 0;
  double* local_n_r = internalMemoryLocal + local_n * 1;
  double* local_Ap = internalMemoryLocal + local_n * 2;
  double* local_p = internalMemoryLocal + local_n * 3;

  double* internalMemoryGlobal = new double[n * 2];
  double* p = internalMemoryGlobal + n * 0;
  double* n_p = internalMemoryGlobal + n * 1;

  std::fill_n(x, n, 1.0);

  MulMatVec(local_Ap, local_A, local_n, n, x);

  for (int i = 0; i < local_n; i++) {
    local_r[i] = local_p[i] = b[i + rank_proc * local_n] - local_Ap[i];
  }
  MPI_Allgather(local_p, local_n, MPI_DOUBLE, p, local_n, MPI_DOUBLE,
                MPI_COMM_WORLD);

  double norm_b = 0.0;
  for (int i = 0; i < n; i++) {
    norm_b += b[i] * b[i];
  }

  int iter = 0;
  for (; iter < max_iter; iter++) {
    MulMatVec(local_Ap, local_A, local_n, n, p);

    double local_nm[2] = {0.0};
    double nm[2] = {0.0};

    for (int i = 0; i < local_n; i++) {
      local_nm[0] += local_r[i] * local_r[i];
      local_nm[1] += p[i + rank_proc * local_n] * local_Ap[i];
    }

    MPI_Allreduce(local_nm, nm, 2, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    const double alpha = nm[0] / nm[1];

    double check = 0.0;

    for (int i = 0; i < n; i++) {
      const double alphaP = alpha * p[i];
      const double x_prev = x[i];
      x[i] += alphaP;
      check += (x[i] - x_prev) * (x[i] - x_prev);
    }

    if (check < eps * eps * norm_b) {
      break;
    }

    for (int i = 0; i < local_n; i++) {
      local_n_r[i] = local_r[i] - alpha * local_Ap[i];
    }

    local_nm[0] = 0.0;
    local_nm[1] = 0.0;
    nm[0] = 0.0;
    nm[1] = 0.0;
    for (int i = 0; i < local_n; i++) {
      local_nm[0] += local_n_r[i] * local_n_r[i];
      local_nm[1] += local_r[i] * local_r[i];
    }

    MPI_Allreduce(local_nm, nm, 2, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    const double beta = nm[0] / nm[1];

    for (int i = 0; i < local_n; i++) {
      local_p[i] = local_n_r[i] + beta * p[i + rank_proc * local_n];
    }
    MPI_Allgather(local_p, local_n, MPI_DOUBLE, n_p, local_n, MPI_DOUBLE,
                  MPI_COMM_WORLD);

    std::swap(local_r, local_n_r);
    std::swap(p, n_p);
  }

  count = iter;

  delete[] internalMemoryLocal;
  delete[] internalMemoryGlobal;
}

void SLE_Solver_Sequentional(const double* const A, const int n,
                             const double* const b, const double eps,
                             const int max_iter, double* x, int& count) {
  double* internalMemory = new double[n * 5];

  double* r = internalMemory + n * 0;
  double* n_r = internalMemory + n * 1;

  double* p = internalMemory + n * 2;
  double* n_p = internalMemory + n * 3;

  double* Ap = internalMemory + n * 4;

  std::fill_n(x, n, 1.0);

  MulMatVec(Ap, A, n, n, x);

  for (int i = 0; i < n; i++) {
    r[i] = p[i] = b[i] - Ap[i];
  }

  double norm_b = 0.0;

  for (int i = 0; i < n; i++) {
    norm_b += b[i] * b[i];
  }

  int iter = 0;
  for (; iter < max_iter; iter++) {
    MulMatVec(Ap, A, n, n, p);

    double numerator = 0.0;
    double denominator = 0.0;

    for (int i = 0; i < n; i++) {
      numerator += r[i] * r[i];
      denominator += p[i] * Ap[i];
    }

    const double alpha = numerator / denominator;

    double check = 0.0;

    for (int i = 0; i < n; i++) {
      const double alphaP = alpha * p[i];
      const double x_prev = x[i];
      x[i] += alphaP;
      check += (x[i] - x_prev) * (x[i] - x_prev);
    }

    if (check < eps * eps * norm_b) {
      break;
    }

    for (int i = 0; i < n; i++) {
      n_r[i] = r[i] - alpha * Ap[i];
    }

    numerator = 0.0;
    denominator = 0.0;
    for (int i = 0; i < n; i++) {
      numerator += n_r[i] * n_r[i];
      denominator += r[i] * r[i];
    }

    const double beta = numerator / denominator;

    for (int i = 0; i < n; i++) {
      n_p[i] = n_r[i] + beta * p[i];
    }

    std::swap(r, n_r);
    std::swap(p, n_p);
  }

  count = iter;

  delete[] internalMemory;
}

#endif  // PARALLEL_ALGORITHMS_CONJGRAD_H
