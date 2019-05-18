//
// Created by kirill on 04.11.17.
//

#include "spline.h"

#include <mkl.h>
#include <math.h>
#include <cstring>
#include <cstdlib>

template class CubicSpline<float>;
template class CubicSpline<double>;

template<typename T> lapack_int lapacke_gtsv(int matrix_layout, lapack_int n, lapack_int nrhs,
                           T* dl, T* d, T* du, T* b, lapack_int ldb) { return 0; }


template<> lapack_int lapacke_gtsv<float>(int matrix_layout, lapack_int n, lapack_int nrhs,
                                  float* dl, float* d, float* du, float* b, lapack_int ldb ) {
    return LAPACKE_sgtsv(matrix_layout, n, nrhs, dl, d, du, b, ldb);
}

template<> lapack_int lapacke_gtsv<double>(int matrix_layout, lapack_int n, lapack_int nrhs,
                                  double* dl, double* d, double* du, double* b, lapack_int ldb ) {
    return LAPACKE_dgtsv(matrix_layout, n, nrhs, dl, d, du, b, ldb);
}


#define DELETE(array)     \
 if (array != nullptr) {  \
    delete[] array;       \
    array = nullptr;      \
  }                       \


template <class type>
CubicSpline<type>::Spline::Spline(int n, type *xin) {
  nPoints = n;
  a = new type[n-1];
  b = new type[n-1];
  c = new type[n-1];
  d = new type[n-1];

  x = new type[n];
  std::memcpy(x, xin, sizeof(type)*n);
}

template <class type>
CubicSpline<type>::Spline::~Spline() {
  nPoints = 0;
  DELETE(a);
  DELETE(b);
  DELETE(c);
  DELETE(d);
  DELETE(x);
}

template <class type>
CubicSpline<type>::CubicSpline(int n, type *x, type *y) {
  _spline = new Spline(n, x);
  type *dl, *d, *du, *hx;
  dl = new type[n-3];
  d = new type[n-2];
  du = (type*)malloc(sizeof(type)*(n-3));
  hx = (type*)malloc(sizeof(type)*(n-1));
  type *rhs = (type*)malloc(sizeof(type)*(n-2));

  for (int i = 0; i < n - 1; i++) {
    hx[i] = x[i+1] - x[i];
  }

  d [0] = 2.f*(hx[0] + hx[1]) - 0.5f*hx[0];
  dl[0] = hx[1];
  du[0] = hx[1];

  for (int i = 1; i < n-2; i++){
    d [i] = 2.f*(hx[i] + hx[i+1]);
    dl[i] = hx[i+1];
    du[i] = hx[i+1];
  }
  d[n-3] = 2.f*(hx[n-3] + hx[n-2]) - 0.5f*hx[n-2];

  for (int i = 0; i < n - 2; i++)
    rhs[i] = 3.f*((y[i+2] - y[i+1])/hx[i+1] - (y[i+1] - y[i])/hx[i]);

  rhs[0]   -= 1.5*((y[1] - y[0])  /hx[0]);
  rhs[n-3] += 1.5*((y[n-2] - y[n-2])/hx[n-2]);

  lapacke_gtsv<type>(LAPACK_COL_MAJOR, n-2, 1, dl, d, du, rhs, n-2);

  for (int i = 0; i < n-2; i++){
    _spline->c[i+1] = rhs[i];
  }

  _spline->c[0] = -0.5f*_spline->c[1] + 1.5f*((y[1] - y[0] )/hx[0])/hx[0];

  for (int i = 0; i < n-1; i++) {
    _spline->d[i] = (_spline->c[i+1] - _spline->c[i])/(3.f*hx[i]);
    _spline->b[i] = (y[i+1]-y[i])/hx[i] - (hx[i]/3.f)*(_spline->c[i+1]+2*_spline->c[i]);
    _spline->a[i] = y[i];
  }

  DELETE(rhs);
  DELETE(d);
  DELETE(dl);
  DELETE(du);
}

template <class type>
CubicSpline<type>::~CubicSpline() {
  delete _spline;
}

template <class type>
type CubicSpline<type>::interpolate(type x) {
  int last = _spline->nPoints - 1;
  int first = 0;
  int mid;

  while (last - first > 1) {
    mid = (int)((last + first)*0.5);
    if (x > _spline->x[mid])
      first = mid;
    else
      last = mid;
  }
  int index = first;

  type y = _spline->a[index] +
      _spline->b[index]*(x - _spline->x[index]) +
      _spline->c[index]*powf(x - _spline->x[index], 2) +
      _spline->d[index]*powf(x - _spline->x[index], 3);

  return y;
}

template class CubicSplineMkl<float>;

template <class type>
CubicSplineMkl<type>::CubicSplineMkl(int n, type *x, type *y) {
  int status;
  status = dfsNewTask1D( &_task, n, x, DF_NON_UNIFORM_PARTITION, 1, y, DF_NO_HINT );

  _scoeff = new type[(n-1)* 4];
  status = dfsEditPPSpline1D(_task, DF_PP_CUBIC, DF_PP_NATURAL, DF_BC_NOT_A_KNOT, NULL, DF_NO_IC,
                              NULL, _scoeff, DF_NO_HINT);
  status = dfsConstruct1D(_task, DF_PP_SPLINE, DF_METHOD_STD);
}

template <class type>
CubicSplineMkl<type>::~CubicSplineMkl() {
  int status = dfDeleteTask(&_task);
  DELETE(_scoeff);
}

template <class type>
type CubicSplineMkl<type>::interpolate(type x) {
  type res;
  int status = dfsInterpolate1D(_task, DF_INTERP, DF_METHOD_PP, 1, &x,
                                DF_NON_UNIFORM_PARTITION, 0, NULL,
                                DF_NO_APRIORI_INFO,
                                &res, DF_NO_HINT, NULL );
  return res;
}


