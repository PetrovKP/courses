//
// Created by kirill on 04.04.19.
//

#include "sde.hpp"

#include <Python.h>
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/npy_math.h>
#include <numpy/arrayobject.h>

#define CHECK_PY_ERROR(err) if (!err) { return NULL; }

static PyObject* euler(PyObject* self, PyObject* args);
static PyObject* heun(PyObject* self, PyObject* args);

static PyMethodDef SDEMethods[] =
{
    {
        "euler", euler, METH_VARARGS, "Calculate sde with method euler."
    },
    {
        "heun", heun, METH_VARARGS, "Calculate sde with method heun."
    },
    {
        NULL, NULL, 0, NULL
    }
};

static PyModuleDef SDEmodule =
{
    PyModuleDef_HEAD_INIT, "SDE", "Modul for solving Stochastic Differential Equations (1-order)", -1, SDEMethods
};

PyMODINIT_FUNC PyInit_SDE()
{
    PyObject* module = PyModule_Create(&SDEmodule);
    CHECK_PY_ERROR(module);
    import_array();
    return module;
}

static PyObject* euler(PyObject* self, PyObject* args)
{
    int n, seed;
    double dt, a, x_start;
    CHECK_PY_ERROR(PyArg_ParseTuple(args, "ddidi", &x_start, &dt, &n, &a, &seed));

    SDE sde_euler(seed, x_start, dt, n, a);

    npy_intp dims[1] = { n };

    PyArrayObject* x_numpy =
        reinterpret_cast<PyArrayObject*>(PyArray_SimpleNew(1, dims, NPY_DOUBLE));
    double* x = static_cast<double*>(PyArray_DATA(x_numpy));

    PyArrayObject* xt_numpy =
        reinterpret_cast<PyArrayObject*>(PyArray_SimpleNew(1, dims, NPY_DOUBLE));
    double* xt = static_cast<double*>(PyArray_DATA(xt_numpy));

    sde_euler.euler(x, xt);

    return Py_BuildValue("(NN)", x_numpy, xt_numpy);
}

static PyObject* heun(PyObject* self, PyObject* args)
{
    int n, seed, n_iter, nThreads;
    double dt, a, x_start, D;
    CHECK_PY_ERROR(PyArg_ParseTuple(args, "ddidiidi", &x_start, &dt, &n, &a, &seed, &n_iter, &D, &nThreads));

    SDE sde_heun(seed, x_start, dt, n, a, n_iter, D, nThreads);

    npy_intp dims[1] = { n };

    PyArrayObject* x_numpy =
        reinterpret_cast<PyArrayObject*>(PyArray_SimpleNew(1, dims, NPY_DOUBLE));
    double* x = static_cast<double*>(PyArray_DATA(x_numpy));

    PyArrayObject* xt_numpy =
        reinterpret_cast<PyArrayObject*>(PyArray_SimpleNew(1, dims, NPY_DOUBLE));
    double* xt = static_cast<double*>(PyArray_DATA(xt_numpy));

    PyArrayObject* prob_numpy =
        reinterpret_cast<PyArrayObject*>(PyArray_SimpleNew(1, dims, NPY_DOUBLE));
    double* prob = static_cast<double*>(PyArray_DATA(prob_numpy));

    sde_heun.heun(x, xt, prob);

    return Py_BuildValue("(NNN)", x_numpy, xt_numpy, prob_numpy);
}

