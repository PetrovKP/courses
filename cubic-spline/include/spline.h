//
// Created by kirill on 04.11.17.
//

#ifndef CUBIC_SPLAIN_SPLINE_H
#define CUBIC_SPLAIN_SPLINE_H

#include <mkl.h>

template <class type>
class CSpline {
 public:
  virtual type interpolate(type x) = 0;
};

template <class type>
class CubicSpline: public CSpline<type> {
 public:
  CubicSpline(int n, type *x, type *y);
  ~CubicSpline();
  type interpolate(type x);

 private:
  struct Spline {
    Spline(int n, type *x);
    ~Spline();
    type *a;
    type *b;
    type *c;
    type *d;
    type *x;
    int nPoints;
  };
  Spline *_spline;
};

template <class type>
class CubicSplineMkl: public CSpline<type> {
 public:
  CubicSplineMkl(int n, type *x, type *y);
  ~CubicSplineMkl();
  type interpolate(type x);

 private:
  DFTaskPtr _task;
  type* _scoeff;
};

#endif //CUBIC_SPLAIN_SPLINE_H
