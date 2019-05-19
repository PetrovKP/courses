#include "interpolation.h"

void updateCoefficients(double (&color)[4][4], double p[4][4]) {
//  color[0][0] = p[1][1];
//  color[0][1] = -.5 * p[1][0] + .5 * p[1][2];
//  color[0][2] = p[1][0] - 2.5 * p[1][1] + 2 * p[1][2] - .5 * p[1][3];
//  color[0][3] = -.5 * p[1][0] + 1.5 * p[1][1] - 1.5 * p[1][2] + .5 * p[1][3];
//  color[1][0] = -.5 * p[0][1] + .5 * p[2][1];
//  color[1][1] = .25 * p[0][0] - .25 * p[0][2] - .25 * p[2][0] + .25 * p[2][2];
//  color[1][2] = -.5 * p[0][0] + 1.25 * p[0][1] - p[0][2] + .25 * p[0][3] + .5 * p[2][0] - 1.25 * p[2][1] + p[2][2]
//      - .25 * p[2][3];
//  color[1][3] =
//      .25 * p[0][0] - .75 * p[0][1] + .75 * p[0][2] - .25 * p[0][3] - .25 * p[2][0] + .75 * p[2][1] - .75 * p[2][2]
//          + .25 * p[2][3];
//  color[2][0] = p[0][1] - 2.5 * p[1][1] + 2 * p[2][1] - .5 * p[3][1];
//  color[2][1] = -.5 * p[0][0] + .5 * p[0][2] + 1.25 * p[1][0] - 1.25 * p[1][2] - p[2][0] + p[2][2] + .25 * p[3][0]
//      - .25 * p[3][2];
//  color[2][2] = p[0][0] - 2.5 * p[0][1] + 2 * p[0][2] - .5 * p[0][3] - 2.5 * p[1][0] + 6.25 * p[1][1] - 5 * p[1][2]
//      + 1.25 * p[1][3] + 2 * p[2][0] - 5 * p[2][1] + 4 * p[2][2] - p[2][3] - .5 * p[3][0] + 1.25 * p[3][1] - p[3][2]
//      + .25 * p[3][3];
//  color[2][3] =
//      -.5 * p[0][0] + 1.5 * p[0][1] - 1.5 * p[0][2] + .5 * p[0][3] + 1.25 * p[1][0] - 3.75 * p[1][1] + 3.75 * p[1][2]
//          - 1.25 * p[1][3] - p[2][0] + 3 * p[2][1] - 3 * p[2][2] + p[2][3] + .25 * p[3][0] - .75 * p[3][1]
//          + .75 * p[3][2] - .25 * p[3][3];
//  color[3][0] = -.5 * p[0][1] + 1.5 * p[1][1] - 1.5 * p[2][1] + .5 * p[3][1];
//  color[3][1] =
//      .25 * p[0][0] - .25 * p[0][2] - .75 * p[1][0] + .75 * p[1][2] + .75 * p[2][0] - .75 * p[2][2] - .25 * p[3][0]
//          + .25 * p[3][2];
//  color[3][2] = -.5 * p[0][0] + 1.25 * p[0][1] - p[0][2] + .25 * p[0][3] + 1.5 * p[1][0] - 3.75 * p[1][1] + 3 * p[1][2]
//      - .75 * p[1][3] - 1.5 * p[2][0] + 3.75 * p[2][1] - 3 * p[2][2] + .75 * p[2][3] + .5 * p[3][0] - 1.25 * p[3][1]
//      + p[3][2] - .25 * p[3][3];
//  color[3][3] =
//      .25 * p[0][0] - .75 * p[0][1] + .75 * p[0][2] - .25 * p[0][3] - .75 * p[1][0] + 2.25 * p[1][1] - 2.25 * p[1][2]
//          + .75 * p[1][3] + .75 * p[2][0] - 2.25 * p[2][1] + 2.25 * p[2][2] - .75 * p[2][3] - .25 * p[3][0]
//          + .75 * p[3][1] - .75 * p[3][2] + .25 * p[3][3];
  color[0][0] = p[0][0];
  color[0][1] = p[2][0];
  color[0][2] = -3*p[0][0] + 3*p[0][2] - 2*p[2][0] - p[2][2];
  color[0][3] = 2*p[0][0] - 2*p[0][2] + p[2][0] + p[2][2];
  color[1][0] = p[1][0];
  color[1][1] = p[3][0];
  color[1][2] = -3*p[1][0] + 3*p[1][2] - 2*p[3][0] - p[3][2];
  color[1][3] = 2*p[1][0] - 2*p[1][2] + p[3][0] + p[3][2];
  color[2][0] = -3*p[0][0] + 3*p[0][1] - 2*p[1][0] -p[1][1];
  color[2][1] = -3*p[2][0] + 3*p[2][1] - 2*p[3][0] -p[3][1];
  color[2][2] = 9*p[0][0] - 9*p[0][1] - 9*p[0][2] + 9*p[0][3] + 6*p[1][0] + 3*p[1][1]
      - 6*p[1][2] - 3*p[1][3] + 6*p[2][0] - 6*p[2][1] + 3*p[2][2] - 3*p[2][3] +
      + 4*p[3][0] + 2*p[3][1] + 2*p[3][2] + p[3][3];
  color[2][3] = -6*p[0][0] + 6*p[0][1] + 6*p[0][2] - 6*p[0][3] - 4*p[1][0] - 2*p[1][1] +
      + 4*p[1][2] + 2*p[1][3] - 3*p[2][0] + 3*p[2][1] - 3*p[2][2] + 3*p[2][3]
      - 2*p[3][0] - 1*p[3][1] - 2*p[3][2] - p[3][3];
  color[3][0] = 2*p[0][0] - 2*p[0][1] + p[1][0] + p[1][1];
  color[3][1] = 2*p[2][0] - 2*p[2][1] + p[3][0] + p[3][1];
  color[3][2] = -6*p[0][0] + 6*p[0][1] + 6*p[0][2] - 6*p[0][3] - 3*p[1][0] - 3*p[1][1]
      + 3*p[1][2] + 3*p[1][3] - 4*p[2][0] + 4*p[2][1] - 2*p[2][2] + 2*p[2][3] +
      - 2*p[3][0] - 2*p[3][1] - 1*p[3][2] - 1*p[3][3];
  color[3][3] = 4*p[0][0] - 4*p[0][1] - 4*p[0][2] + 4*p[0][3] + 2*p[1][0] + 2*p[1][1]
      - 2*p[1][2] - 2*p[1][3] + 2*p[2][0] - 2*p[2][1] + 2*p[2][2] - 2*p[2][3] +
      + 1*p[3][0] + 1*p[3][1] + 1*p[3][2] + 1*p[3][3];
}

double getValue(double color[4][4], double x, double y) {
  double x2 = x * x;
  double x3 = x2 * x;
  double y2 = y * y;
  double y3 = y2 * y;

  return (color[0][0] + color[0][1] * y + color[0][2] * y2 + color[0][3] * y3) +
      (color[1][0] + color[1][1] * y + color[1][2] * y2 + color[1][3] * y3) * x +
      (color[2][0] + color[2][1] * y + color[2][2] * y2 + color[2][3] * y3) * x2 +
      (color[3][0] + color[3][1] * y + color[3][2] * y2 + color[3][3] * y3) * x3;
}

BicubicInterpolation::BicubicInterpolation(const cv::Mat &mat, int coeff) {
  mat_ = mat;
  coeff_ = coeff;
}

cv::Mat BicubicInterpolation::rescale() {
  cv::Mat newMat = cv::Mat(mat_.rows * coeff_, mat_.cols * coeff_, mat_.type());

  double area[4][4];

  for (int i = 0; i < mat_.rows; i++) {
    for (int j = 0; j < mat_.cols; j++) {
      // generate coeffs for bicubic interpolation

      area[0][0] = mat_.at<cv::Vec3b>(i + 0, j + 0)[0];
      area[0][1] = mat_.at<cv::Vec3b>(i + 0, j + 1)[0];
      area[0][2] = mat_.at<cv::Vec3b>(i + 0, j + 2)[0];
      area[0][3] = mat_.at<cv::Vec3b>(i + 0, j + 3)[0];
      area[1][0] = mat_.at<cv::Vec3b>(i + 1, j + 0)[0];
      area[1][1] = mat_.at<cv::Vec3b>(i + 1, j + 1)[0];
      area[1][2] = mat_.at<cv::Vec3b>(i + 1, j + 2)[0];
      area[1][3] = mat_.at<cv::Vec3b>(i + 1, j + 3)[0];
      area[2][0] = mat_.at<cv::Vec3b>(i + 2, j + 0)[0];
      area[2][1] = mat_.at<cv::Vec3b>(i + 2, j + 1)[0];
      area[2][2] = mat_.at<cv::Vec3b>(i + 2, j + 2)[0];
      area[2][3] = mat_.at<cv::Vec3b>(i + 2, j + 3)[0];
      area[3][0] = mat_.at<cv::Vec3b>(i + 3, j + 0)[0];
      area[3][1] = mat_.at<cv::Vec3b>(i + 3, j + 1)[0];
      area[3][2] = mat_.at<cv::Vec3b>(i + 3, j + 2)[0];
      area[3][3] = mat_.at<cv::Vec3b>(i + 3, j + 3)[0];

      updateCoefficients(r, area);

      area[0][0] = mat_.at<cv::Vec3b>(i + 0, j + 0)[1];
      area[0][1] = mat_.at<cv::Vec3b>(i + 0, j + 1)[1];
      area[0][2] = mat_.at<cv::Vec3b>(i + 0, j + 2)[1];
      area[0][3] = mat_.at<cv::Vec3b>(i + 0, j + 3)[1];
      area[1][0] = mat_.at<cv::Vec3b>(i + 1, j + 0)[1];
      area[1][1] = mat_.at<cv::Vec3b>(i + 1, j + 1)[1];
      area[1][2] = mat_.at<cv::Vec3b>(i + 1, j + 2)[1];
      area[1][3] = mat_.at<cv::Vec3b>(i + 1, j + 3)[1];
      area[2][0] = mat_.at<cv::Vec3b>(i + 2, j + 0)[1];
      area[2][1] = mat_.at<cv::Vec3b>(i + 2, j + 1)[1];
      area[2][2] = mat_.at<cv::Vec3b>(i + 2, j + 2)[1];
      area[2][3] = mat_.at<cv::Vec3b>(i + 2, j + 3)[1];
      area[3][0] = mat_.at<cv::Vec3b>(i + 3, j + 0)[1];
      area[3][1] = mat_.at<cv::Vec3b>(i + 3, j + 1)[1];
      area[3][2] = mat_.at<cv::Vec3b>(i + 3, j + 2)[1];
      area[3][3] = mat_.at<cv::Vec3b>(i + 3, j + 3)[1];

      updateCoefficients(g, area);

      area[0][0] = mat_.at<cv::Vec3b>(i + 0, j + 0)[2];
      area[0][1] = mat_.at<cv::Vec3b>(i + 0, j + 1)[2];
      area[0][2] = mat_.at<cv::Vec3b>(i + 0, j + 2)[2];
      area[0][3] = mat_.at<cv::Vec3b>(i + 0, j + 3)[2];
      area[1][0] = mat_.at<cv::Vec3b>(i + 1, j + 0)[2];
      area[1][1] = mat_.at<cv::Vec3b>(i + 1, j + 1)[2];
      area[1][2] = mat_.at<cv::Vec3b>(i + 1, j + 2)[2];
      area[1][3] = mat_.at<cv::Vec3b>(i + 1, j + 3)[2];
      area[2][0] = mat_.at<cv::Vec3b>(i + 2, j + 0)[2];
      area[2][1] = mat_.at<cv::Vec3b>(i + 2, j + 1)[2];
      area[2][2] = mat_.at<cv::Vec3b>(i + 2, j + 2)[2];
      area[2][3] = mat_.at<cv::Vec3b>(i + 2, j + 3)[2];
      area[3][0] = mat_.at<cv::Vec3b>(i + 3, j + 0)[2];
      area[3][1] = mat_.at<cv::Vec3b>(i + 3, j + 1)[2];
      area[3][2] = mat_.at<cv::Vec3b>(i + 3, j + 2)[2];
      area[3][3] = mat_.at<cv::Vec3b>(i + 3, j + 3)[2];

      updateCoefficients(b, area);

      for (int k = 0; k < coeff_; k++) {
        for (int l = 0; l < coeff_; l++) {
//          uchar valueR = mat_.at<cv::Vec3b>(i, j)[0];
//          uchar valueG = mat_.at<cv::Vec3b>(i, j)[1];
//          uchar valueB = mat_.at<cv::Vec3b>(i, j)[2];

          double valueR = getValue(r, (double) k / coeff_, (double) l / coeff_);
          double valueG = getValue(g, (double) k / coeff_, (double) l / coeff_);
          double valueB = getValue(b, (double) k / coeff_, (double) l / coeff_);
          cv::Vec3d color(valueR, valueG, valueB);

          newMat.at<cv::Vec3b>(coeff_*i + k, coeff_*j + l) = color;
        }
      }

    }
  }
  return newMat;
}