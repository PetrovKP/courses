//
// Created by kirill on 27.04.19.
//

#include <iostream>
#include <algorithm>
#include <omp.h>
#include "util.h"
#include "runge-kutta.h"

constexpr double pi = std::acos(-1.0);
double u(double x, double t) { return std::sin(x*pi)*std::exp(-pi*pi*t);}

class HeatTaskMain : public heat_task
{
public:
    double initial_condition(double x) { return u(x, 0); }

    double left_condition(double t) { return 0; }

    double right_condition(double t) { return 0; }

    double f(double x, double t) { return 0.0; }
};

int main(int argc, char *argv[])
{
    int threads[] = {1};
    int n_array[] = {20};
    int m_array[] = {10000};

    double T = 0.3;
    double L = 1.0;

    for (int n : n_array)
    {
        for (int m : m_array)
        {

            HeatTaskMain task;
            task.n = n;
            task.m = m;
            task.L = L;
            task.T = T;

            for (int thread: threads)
            {
                omp_set_num_threads(thread);
                double *v = new double[n + 1];

                double ts = omp_get_wtime();
                heat_equation_runge_kutta(task, v);
                double tf = omp_get_wtime();

                printf("n %d\t m %d\t nThread %d\t Time %f ms\n", n, m, thread, (tf - ts)*1000);
                PrintArray(v, n + 1);

                for (int i = 0; i <= n; i++)
                {
                    printf("%.3f ", u((double)i/(double)task.n, T));
                }
                printf("\n");

                delete[] v;
            }
        }
    }
}


