//
// Created by kirill on 27.04.19.
//

#include <cstdlib>
#include <cmath>
#include <omp.h>
#include "stdio.h"
#include "assert.h"

// CSR (Compressed Sparse Rows)
struct SpMatrix
{
    double *value; // Элементы матрицы
    int *col;      // Номера столбцов для каждого элемента
    int *row_index; // Место каждого ненулевого элемента в каждой строке
    int nz;        // Количество ненулевых элементов
    int nRows;     // Количество строк

    SpMatrix(int _nz, int _nRows)
    {
        nz = _nz;
        nRows = _nRows;
        value = (double *) malloc(sizeof(double) * nz);
        col = (int *) malloc(sizeof(int) * nz);
        row_index = (int *) calloc(nRows + 1, sizeof(int));
    }

    ~SpMatrix()
    {
        free(value);
        free(col);
        free(row_index);
    }

    void createExplicitSpMat(double coeff)
    {
        int index = 0, k = 0;
        row_index[0] = 0;

        // For idx = 0
        col[index] = 0;
        value[index] = -2.0*coeff;
        index++;

        col[index] = 1;
        value[index] = coeff;
        index++;

        k++;
        row_index[k] = row_index[k - 1] + 2;
        //

        for (int idx = 1; idx < nRows - 1; idx++)
        {
            // ***************************************
            //      x - 1
            // ***************************************
            col[index] = idx - 1;
            value[index] = coeff;
            index++;

            // ***************************************
            //     x
            // ***************************************
            col[index] = idx;
            value[index] = -2.0*coeff;
            index++;


            // ***************************************
            //     x + 1
            // ***************************************
            col[index] = idx + 1;
            value[index] = coeff;
            index++;

            k++;
            row_index[k] = row_index[k - 1] + 3;

        }

        // For idx = nx - 1
        col[index] = nRows - 2;
        value[index] = coeff;
        index++;

        col[index] = nRows - 1;
        value[index] = -2.0*coeff;
        index++;

        k++;
        row_index[k] = row_index[k - 1] + 2;

        assert(k == nRows);
        assert(index == nz);
    }
};

void multSpMV(double *result, SpMatrix &mat, double *u)
{
    double localSum;
    #pragma omp parallel private(localSum)
    {
        #pragma omp for nowait
        for (int i = 0; i < mat.nRows; i++)
        {
            localSum = 0.0;
            for (int j = mat.row_index[i]; j < mat.row_index[i + 1]; j++)
            {
                localSum += mat.value[j] * u[mat.col[j]];
            }
            result[i] = localSum;
        }
    }
}

// z = x + y*alpha
void daxpy(double *z, const int n, const double * const x,
    const double * const y, const double alpha)
{
    for (int i = 0; i < n; i++)
    {
        z[i] = x[i] + y[i]*alpha;
    }

}


#ifdef USE_HEADER
template <typename TaskType>
void heat_equation_runge_kutta(TaskType task, double *v)
#else
void heat_equation_runge_kutta(heat_task task, double *v)
#endif
{

    if(task.n == 1)
    {
        v[0] = task.left_condition(task.T);
        v[1] = task.right_condition(task.T);
        return;
    }

    int n = task.n + 1;

    const double h = task.L / task.n;
    const double tau = task.T / task.m;

    const int nz = 3*(task.n - 3) + 2*2;
    SpMatrix spmat(nz, task.n - 1);

    const double inv_h2 = 1.0/(h*h);

    spmat.createExplicitSpMat(inv_h2);

    double *k1 = (double *) malloc(sizeof(double) * n);
    double *k2 = (double *) malloc(sizeof(double) * n);
    double *k3 = (double *) malloc(sizeof(double) * n);
    double *k4 = (double *) malloc(sizeof(double) * n);

    double *tmp = (double *) malloc(sizeof(double) * n);

    for (int i = 0; i < n; i++)
    {
        v[i] = task.initial_condition(i*h);
    }

    for (double t = tau; t <= task.T; t+=tau)
    {

        multSpMV(k1 + 1, spmat, v + 1);

        k1[0] = task.left_condition(t);
        k1[n - 1] = task.right_condition(t);

        for (int i = 1; i < n - 1; i++)
        {
            k1[i] = k1[i] + task.f(i*h, t);
        }

        daxpy(tmp, n, v, k1, tau*0.5);

        multSpMV(k2 + 1, spmat, tmp + 1);

        k2[0] = task.left_condition(t);
        k2[n - 1] = task.right_condition(t);

        for (int i = 1; i < n-1; i++)
        {
            k2[i] = k2[i] + task.f(i*h, t);
        }

        daxpy(tmp, n, v, k2, tau*0.5);

        multSpMV(k3 + 1, spmat, tmp + 1);
        for (int i = 1; i < n-1; i++)
        {
            k3[i] = k3[i] + task.f(i*h, t);
        }
        k3[0] = task.left_condition(t);
        k3[n - 1] = task.right_condition(t);

        daxpy(tmp, n, v, k3, tau);

        multSpMV(k4 + 1, spmat, tmp + 1);
        for (int i = 1; i < n-1; i++)
        {
            k4[i] = k4[i] + task.f(i*h, t);
        }
        k4[0] = task.left_condition(t);
        k4[n - 1] = task.right_condition(t);

        for (int i = 0; i < n; i++)
        {
            v[i] = v[i] + tau * (k1[i] + 2.0*k2[i] + 2.0*k3[i] + k4[i])/6.0;
        }

    }

    free(k1);
    free(k2);
    free(k3);
    free(k4);

    free(tmp);

}
