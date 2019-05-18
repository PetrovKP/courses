//
// Created by kirill on 27.10.18.
//

#include <catch.hpp>

#include "runge-kutta.h"
#include "util.h"

#include <iostream>
#include <algorithm>
#include <string>
#include <set>
#include <functional>
#include <util.h>
#include <limits>
#include <ctime>
#include <cmath>

constexpr double pi = std::acos(-1.0);

static void Check(const double *v, const double *expected, const int n)
{
    for (int i = 0; i < n; i++)
    {
        // printf("%d %lf\n",i, v[i]);
        REQUIRE(std::abs(v[i] - expected[i]) < 1e-5);
    }
}

double u(double x, double t) { return std::sin(x*pi)*std::exp(-pi*pi*t);}

TEST_CASE("Simple")
{

    class HeatSin : public heat_task
    {
    public:
        double initial_condition(double x) { return u(x, 0); }

        double left_condition(double t) { return 0; }

        double right_condition(double t) { return 0; }

        double f(double x, double t) { return 0.0; }
    };

    HeatSin task;
    task.n = 512;
    task.m = 131072;
    task.L = 1.0;
    task.T = 0.01;

    double *v = new double[task.n + 1];
    double *expected = new double[task.n + 1];

    {
        heat_equation_runge_kutta(task, v);

        for (int i = 0; i < task.n + 1; i++)
        {
            expected[i] = u((double)i/(double)task.n, task.T);
        }
        Check(v, expected, task.n + 1);
    }

    delete [] v;
    delete [] expected;
}