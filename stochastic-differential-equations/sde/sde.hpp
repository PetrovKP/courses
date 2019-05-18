//
// Created by kirill on 04.04.19.
//

#ifndef _SDE_HPP__
#define _SDE_HPP__

#include <cstdlib>
#include <cmath>
#include <omp.h>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <functional>

#include "mkl.h"

template<typename T>
int vRngGaussian(const int method, VSLStreamStatePtr stream, const int n, T* r, const T mean, const T sigma)
{ return -1; }

template<>
int vRngGaussian<float>(const int method, VSLStreamStatePtr stream, const int n, float* r, const float mean, const float sigma)
{
    return vsRngGaussian(method, stream, n, r, mean, sigma);
}

template<>
int vRngGaussian<double>(const int method, VSLStreamStatePtr stream, const int n, double* r, const double mean, const double sigma)
{
    return vdRngGaussian(method, stream, n, r, mean, sigma);
}

#define CHECK_ERROR_MKL(err) if (err) std::runtime_error("MKL FUNCTION ERROR: " + std::to_string(err));


template<typename T>
class SDE
{
public:
    SDE(const std::uint32_t seed, const T x0, const T h, const std::uint32_t n, const T a,
            const std::uint32_t n_iter = 1u, const T D = T(1), const std::uint32_t nthreads = 1)
    {
        this->nthreads = nthreads;
        omp_set_num_threads(nthreads);
        this->x0 = x0; this->h = h; this->n = n; this->n_iter = n_iter; this->D = D;
        func = [a](const T x)
        {
            return -std::sin(x) + a;
        };

        vslNewStream(&_stream, VSL_BRNG_PHILOX4X32X10, seed);

        // N(0, 1)
        _noises = static_cast<T*>(mkl_malloc(n * sizeof(T), aligment));
    }

    void euler(T* x, T* xt)
    {
        x[0] = x0;

        CHECK_ERROR_MKL(vRngGaussian(VSL_RNG_METHOD_GAUSSIAN_ICDF, _stream, n, _noises, _mean, _sigma));
        xt[0] = func(x0) + _noises[0];

        for (std::uint32_t i = 0; i < n - 1; i++)
        {
            const T z = std::sqrt(h) * _noises[i];
            // x(h) - x(0) = h*func(x(0)) + z(h)
            x[i + 1] = x[i] + h * func(x[i]) + z;
            xt[i + 1] = func(x[i + 1]) + _noises[i + 1];
        }
    }

    void heun(T* x, T* xt, T* prob)
    {
        T* buff_noise = static_cast<T*>(mkl_malloc(nthreads * n * sizeof(T), aligment));
        T* buff_x     = static_cast<T*>(mkl_calloc(nthreads * n, sizeof(T), aligment));
        T* buff_xt    = static_cast<T*>(mkl_calloc(nthreads * n, sizeof(T), aligment));
        T* buff_prob  = static_cast<T*>(mkl_calloc(nthreads * n, sizeof(T), aligment));

        const T alpha = T(1) / T(n_iter);

        #pragma omp parallel
        {
            #pragma omp for
            for (std::uint32_t iter = 0; iter < n_iter; iter++)
            {
                std::uint32_t tid = omp_get_thread_num();

                VSLStreamStatePtr stream;
                vslCopyStream(&stream, _stream);
                vslSkipAheadStream(stream, iter*n);

                T* local_noise = buff_noise + n*tid;
                T* local_x = buff_x + n*tid;
                T* local_xt = buff_xt + n*tid;
                T* local_prob = buff_prob + n*tid;

                CHECK_ERROR_MKL(vRngGaussian(VSL_RNG_METHOD_GAUSSIAN_ICDF, stream, n, local_noise, _mean, _sigma));
                T xp = x0;
                local_x[0] += x0;
                local_xt[0] += func(xp) + local_noise[0];

                if (-PI < xp && xp < PI) local_prob[0]++;

                for (std::uint32_t i = 0; i < n - 1; i++)
                {
                    const T z = std::sqrt(h * D) * local_noise[i];
                    const T euler = xp + h * func(xp) + z;
                    // x(1) = x(0) + z(h) + h*func(x(0))
                    // x(h) - x(0) = z(h) + 0.5*h*(func(x(0)) + funct(x(1)))
                    xp = xp + z + T(0.5) * h * (func(xp) +  func(euler));
                    local_x[i + 1] += xp;
                    local_xt[i + 1] += func(xp) + local_noise[i + 1];

                    if (-PI < xp && xp < PI) local_prob[i + 1]++;
                }
                vslDeleteStream(&stream);
            }

            std::uint32_t tid = omp_get_thread_num();
            std::uint32_t istart = tid * (n/nthreads+1);
            std::uint32_t iend = (tid+1) * (n/nthreads+1) > n ? n : (tid+1) * (n/nthreads+1);

            for (std::uint32_t i = istart; i < iend; ++i)
            {
                x[i] = alpha*buff_x[i];
                xt[i] = alpha*buff_xt[i];
                prob[i] = alpha*buff_prob[i];
            }

            for (std::uint32_t t = 1; t < nthreads; t++)
            {
                for(std::uint32_t i = istart; i < iend; i++)
                {
                    x[i] += alpha*buff_x[t * n + i];
                    xt[i] += alpha*buff_xt[t * n + i];
                    prob[i] += alpha*buff_prob[t * n + i];
                }
            }
        }

        mkl_free(buff_noise);
        mkl_free(buff_x);
        mkl_free(buff_xt);
        mkl_free(buff_prob);
    }

    ~SDE()
    {
        vslDeleteStream(&_stream);
        mkl_free(_noises);
    }

private:
    T x0;
    T h;
    std::uint32_t n;
    std::uint32_t n_iter;
    T D;
    std::uint32_t nthreads;
    std::function<T(const T)> func;

    T* _noises;
    constexpr static T _mean = T(0.0);
    constexpr static T _sigma = T(1.0);
    constexpr static std::uint32_t aligment = 64u;
    const T PI = std::acos(-1.0);
    VSLStreamStatePtr _stream;
};

#endif //_SDE_HPP__
