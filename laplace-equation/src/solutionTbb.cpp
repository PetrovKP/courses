//
// Created by kirill on 29.03.17.
//

#include <algorithm>
#include <tbb/scalable_allocator.h>
#include "tbb/partitioner.h"

#include "solutionTbb.h"

int parallelTBB(Setting *setting, double *u) {
  int count = 0;
  int N = (int)setting->DIM + 2;
  double EPS = setting->EPS;
  double dmax;

  double *dmaxs = (double *)malloc(sizeof(double)*N);
  do {
    count++;
    // нарастание волны (nx – размер волны)
    for (int nx = 1; nx < N - 1; nx++ ) {
      dmaxs[nx] = 0;

      tbb::parallel_for(tbb::blocked_range<int>(1, nx + 1),
                        [u, nx, dmaxs, N](const tbb::blocked_range<int>& r) {
                          int j;
                          double temp, d;
                          int begin = r.begin(), end = r.end();

                          for (int i = begin; i < end; i++) {
                            j = nx + 1 - i;
                            temp = u[i * N + j];
                            u[i * N + j] = 0.25 * (u[(i - 1) * N + j] +
                                u[(i + 1) * N + j] + u[i * N + j - 1] +
                                u[i * N + j + 1]);
                            d = fabs(temp - u[i * N + j]);
                            if (dmaxs[i] < d) dmaxs[i] = d;
                          }

                        },
                        tbb::static_partitioner());
    }
    // затухание волны
    for (int nx = N - 3; nx > 0; nx-- ) {

      tbb::parallel_for(tbb::blocked_range<int>(N - nx - 1, N - 1),
                      [u, nx, dmaxs, N](const tbb::blocked_range<int>& r) {
                        int j;
                        double temp, d;
                        int begin = r.begin(), end = r.end();

                        for ( int i = begin; i < end; i++ ) {
                          j = 2*(N - 2) - nx - i + 1;
                          temp = u[i*N + j];
                          u[i*N + j] = 0.25*(u[(i - 1)*N + j] +
                              u[(i + 1)*N + j] + u[i*N + j - 1] +
                              u[i*N + j + 1]);
                          d = fabs(temp - u[i*N + j]);
                          if ( dmaxs[i] < d ) dmaxs[i] = d;
                        }
                      },
                        tbb::static_partitioner());
    }
    // сбор максимума
    dmax = tbb::parallel_reduce(tbb::blocked_range<int>(1, N - 1), 0.0,
                                [=](tbb::blocked_range<int>&r, double init) -> double {
                                  int begin = r.begin(), end = r.end();
                                  for ( int i = begin; i < end; i++ ) {
                                    if ( init < dmaxs[i] )
                                      init = dmaxs[i];
                                  }
                                  return init;
                                },
                                [](double x1, double x2) -> double {
                                  return x1 > x2 ? x1 : x2;
                                },
                                tbb::static_partitioner());

  } while ( dmax > EPS );
  free(dmaxs);
  return count;
}