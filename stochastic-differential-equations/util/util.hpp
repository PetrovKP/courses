#ifndef __UTILS_HPP_
#define __UTILS_HPP_

#include <random>
#include <vector>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <iomanip>

template<typename T>
static inline void PrintMatrix(const T *const A, const size_t n, const size_t m, const std::string& message = "")
{
    std::cout << std::setiosflags(std::ios::left);
    std::cout << message << std::endl;
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < m; j++)
        {
            std::cout << std::setw(10) << std::setiosflags(std::ios::fixed) << std::setprecision(4);
            std::cout << A[i*m + j];
        }
        std::cout << std::endl;
    }
}

template<typename T>
static inline void PrintArray(const T *const v, const size_t n, const std::string& message = "")
{
    PrintMatrix(v, 1, n, message);
}

#endif // __UTILS_HPP_