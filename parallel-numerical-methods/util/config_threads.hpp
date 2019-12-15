//
// Created by kirill on 30.04.19.
//

#ifndef PARALLEL_ALGORITHMS_CONFIG_THREADS_HPP
#define PARALLEL_ALGORITHMS_CONFIG_THREADS_HPP

#include <stdio.h>
#include <stdlib.h>

#ifdef USE_TBB
#include <tbb/tbb.h>
#else
#include <omp.h>
#endif

static inline void GetInfo() {
#ifdef USE_TBB
  printf("USED TBB: %d\n", tbb::TBB_runtime_interface_version());
#else
  printf("USED OMP: %u\n", _OPENMP);
#endif
}

static inline auto GetTime() {
#ifdef USE_TBB
  return tbb::tick_count::now();
#else
  return omp_get_wtime();
#endif
}

template <typename T>
static inline auto GetMS(const T& t1, const T& t2) {
#ifdef USE_TBB
  return (t2 - t1).seconds() * 1000.0;
#else
  return (t2 - t1) * 1000.0;
#endif
}

#ifdef USE_TBB
#define SetNThreads(n_threads) tbb::task_scheduler_init init(n_threads);
#else
#define SetNThreads(n_threads) omp_set_num_threads(n_threads);
#endif

#endif  // PARALLEL_ALGORITHMS_CONFIG_THREADS_HPP
