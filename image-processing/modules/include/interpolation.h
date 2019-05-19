#ifndef BINARIZATION_INTERPOLATION_H
#define BINARIZATION_INTERPOLATION_H

#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>

class BicubicInterpolation {
 private:
  cv::Mat mat_;
  int coeff_;
  double r[4][4], g[4][4], b[4][4];
 public:
  BicubicInterpolation(const cv::Mat &mat, int coeff);
  cv::Mat rescale();
};

#endif //BINARIZATION_INTERPOLATION_H
