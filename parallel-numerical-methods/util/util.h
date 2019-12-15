#pragma once

#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

class RandomGenerator;

static inline void PrintArray(const double* const v, const int n) {
  for (int i = 0; i < n; i++) {
    std::cout << v[i] << " ";
  }
  std::cout << "\n";
}

static inline void PrintMatrix(const double* const A, const int n) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      std::cout << A[i * n + j] << " ";
    }
    std::cout << "\n";
  }
}

static inline double CheckDiff(const double* const x, const double* const y,
                               const int n) {
  double max_diff = 0.0;
  for (size_t i = 0UL; i < n; i++) {
    double dist = std::abs(x[i] - y[i]);
    if (std::abs(y[i]) > 1.0) {
      dist = std::min(dist, dist / std::abs(y[i]));
    }
    max_diff = std::max(max_diff, dist);
  }
  return max_diff;
}

enum class Uplo { upper, lower };

static inline void Syrk(double* result, const double* const A, int n,
                        const Uplo uplo) {
  for (int i = 0; i < n; i++) {
    for (int j = i; j < n; j++) {
      double sum = 0.0;
      for (int k = 0; k < (uplo == Uplo::upper ? n : j + 1); k++) {
        sum += A[i * n + k] * A[j * n + k];
      }
      result[i * n + j] = sum;
      result[j * n + i] = sum;
    }
  }
}

static inline void ReferenceMulMatVec(double* result, const double* const A,
                                      const int n, const double* const b) {
  for (int i = 0; i < n; i++) {
    result[i] = 0.0;
    for (int j = 0; j < n; j++) {
      result[i] += A[i * n + j] * b[j];
    }
  }
}

template <typename T>
static inline void GeneratePositiveDefiniteMatrix(T* A, const int n,
                                                  RandomGenerator& random) {
  auto randomB = random.GenRealVector(n * n, T(-5.0), T(5.0));
  const T* const B = randomB.data();

  // B*B^T > 0
  Syrk(A, B, n, Uplo::upper);
}

class RandomGenerator {
 public:
  explicit RandomGenerator(uint32_t seed = 738547485u) : gen_(seed) {}

  template <class T>
  std::vector<T> GenIntegralVector(size_t count, T from, T to) {
    std::uniform_int_distribution<T> dist(from, to);
    std::vector<T> result(count);
    for (auto& cur : result) {
      cur = dist(gen_);
    }
    return result;
  }

  std::string GenString(size_t count, char from = 'a', char to = 'z') {
    std::uniform_int_distribution<char> dist(from, to);
    std::string result(count, from);
    for (char& x : result) {
      x = dist(gen_);
    }
    return result;
  }

  template <class T>
  std::vector<T> GenRealVector(size_t count, T from, T to) {
    std::uniform_real_distribution<T> dist(from, to);
    std::vector<T> result(count);
    for (auto& cur : result) {
      cur = dist(gen_);
    }
    return result;
  }

  std::vector<int> GenPermutation(size_t count) {
    std::vector<int> result(count);
    std::iota(result.begin(), result.end(), 0);
    std::shuffle(result.begin(), result.end(), gen_);
    return result;
  }

  template <class T>
  T GenInt(T from, T to) {
    std::uniform_int_distribution<T> dist(from, to);
    return dist(gen_);
  }

  template <class T>
  T GenFloat(T from, T to) {
    std::uniform_real_distribution<T> dist(from, to);
    return dist(gen_);
  }

  template <class T>
  T GenInt() {
    return GenInt(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
  }

  template <class Iterator>
  void Shuffle(Iterator first, Iterator last) {
    std::shuffle(first, last, gen_);
  }

 private:
  std::mt19937 gen_;
};
