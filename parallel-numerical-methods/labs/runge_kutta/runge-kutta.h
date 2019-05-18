//
// Created by kirill on 27.04.19.
//

#ifndef PARALLEL_ALGORITHMS_RUNGE_KUTTA_H
#define PARALLEL_ALGORITHMS_RUNGE_KUTTA_H

class heat_task
{
public:
    double T; // момент времени, в который необходимо аппроксимировать u(x, t)
    double L; // длина стержня
    int n; // размер сетки по x
    int m; // размер сетки по t
    double initial_condition(double x) {}; // функция, задающая начальное условие
    double left_condition (double t) {};   // функция, задающая граничное условие при x = 0
    double right_condition (double t) {};  // функция, задающая граничное условие при x = L
    double f(double x, double t) {};       // функция, задающая внешнее воздействие
};

template <typename TaskType>
void heat_equation_runge_kutta(TaskType task, double * v);

#include "runge-kutta_impl.i"

#endif //PARALLEL_ALGORITHMS_RUNGE_KUTTA_H
