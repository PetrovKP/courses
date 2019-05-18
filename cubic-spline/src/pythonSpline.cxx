//
// Created by kirill on 05.11.17.
//

#include "spline.h"
#include <boost/python.hpp>
#include <boost/python/extract.hpp>
#include <boost/python/numeric.hpp>
#include <boost/python/list.hpp>

namespace bp = boost::python;

class pythonSpline {
 public:
  pythonSpline(bp::list xPy, bp::list yPy) {
    int nx = len(xPy);
    int ny = len(yPy);
    if (nx != ny)
      return;
    float *x = new float[nx];
    float *y = new float[nx];
    for (int i = 0; i < nx; i++) {
      x[i] = bp::extract<float>(xPy[i]);
      y[i] = bp::extract<float>(yPy[i]);
    }
    spline = new CubicSpline<float>(nx, x, y);
    delete []x;
    delete []y;
  }
  ~pythonSpline() {
    delete spline;
  }
  float interpolate(float x){
    return spline->interpolate(x);
  }
 private:
  CSpline<float>* spline;
};

BOOST_PYTHON_MODULE(spline) {
  // bp::numeric::array::set_module_and_type("numpy", "ndarray");
  bp::class_<pythonSpline>("spline", bp::init<bp::list, bp::list>())
    .def("interpolate", &pythonSpline::interpolate);
}
