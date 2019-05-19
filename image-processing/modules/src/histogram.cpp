//
// Created by ovcharuk on 02.03.17.
//

#include <iostream>
#include "histogram.h"

const uchar Histogram::BLACK = 0;
const uchar Histogram::WHITE = 255;

Histogram::Histogram(const cv::Mat &mat) {
  mat_ = mat.clone();
  hist_ = generateHist(mat_);
}

cv::Mat Histogram::generateHist(const cv::Mat &mat) {
  maxIntensity_ = 0;
  std::fill(intensities_, intensities_ + 256, 0);

  uchar intensity;
  for (int i = 0; i < mat.rows; i++) {
    for (int j = 0; j < mat.cols; j++) {
      intensity = mat.at<uchar>(i, j);

      uchar color = intensity;

      intensities_[color]++;
      if (intensities_[color] > maxIntensity_) {
        maxIntensity_ = intensities_[color];
      }
    }
  }

  cv::Mat hist = cv::Mat(256, 256, mat.type());
  for (int i = 0; i < hist.cols; i++) {
    int edge = calcNewValue(intensities_[i], maxIntensity_);

    for (int j = 0; j < hist.rows; j++) {
      hist.at<uchar>(255 - j, i) = (j > edge) ? BLACK : WHITE;
    }
  }
  return hist;
}

int Histogram::calcNewValue(int value, int max) const {
  int result = (int) (((double) value / max) * (255));
  return result;
}

void Histogram::showHist(const std::string &name) {
  cv::imshow(name, hist_);
}

void Histogram::printIntensities() const {
  for (int i = 0; i < 256; i++)
    std::cout << i << " -> " << intensities_[i] << std::endl;
}

int Histogram::maxValBin() const{
  int maxValue = 0;
  int edge;
  for (int i = 0; i < 256; i++) {
    edge = calcNewValue(intensities_[i], maxIntensity_);
    if (edge > maxValue) {
      maxValue = edge;
    }
  }
  return maxValue;
}

int Histogram::minValBin() const {
  int edge;
  int minValue = std::numeric_limits<int>::max();
  for (int i = 0; i < 256; i++) {
    edge = calcNewValue(intensities_[i], maxIntensity_);
    if (edge < minValue && intensities_[i] > 0) {
      minValue = edge;
    }
  }
  return minValue;
}

int Histogram::maxValIntensity() const {

  int maxValue = 0;
  int maxValueIndex = 0;
  for (int i = 0; i < 256; i++) {
    if (intensities_[i] > maxValue) {
      maxValue = intensities_[i];
      maxValueIndex = i;
    }
  }
  return maxValueIndex;
}

int Histogram::minValIntensity() const {
  int minValue = std::numeric_limits<int>::max();
  int minValueIndex = std::numeric_limits<int>::max();
  for (int i = 0; i < 256; i++) {
    if ((intensities_[i] < minValue) && (intensities_[i] != 0)) {
      minValue = intensities_[i];
      minValueIndex = i;
    }
  }
  return minValueIndex;
}

int Histogram::at(int val) const {
  return intensities_[val];
}

void Histogram::smooth() {
  const int MAX_INTENSIES = 256;
  int intensities[MAX_INTENSIES];
  int edge;
  
  for (int i = 1; i < MAX_INTENSIES - 1; i++)
    intensities[i] = (int)((float)(intensities_[i - 1] + intensities_[i] + intensities_[i + 1])/3.f);

  std::copy(intensities + 1, intensities + MAX_INTENSIES -1, intensities_ + 1);

  maxIntensity_ = *std::max_element(intensities_, intensities_ + MAX_INTENSIES);

//  Перестроение гистограммы
  cv::Mat hist = cv::Mat(MAX_INTENSIES, MAX_INTENSIES, mat_.type());
  for (int i = 0; i < hist.cols; i++) {
    edge = calcNewValue(intensities_[i], maxIntensity_);
    for (int j = 0; j < hist.rows; j++) {
      hist.at<cv::Vec3b>(255 - j, i) = (j > edge) ? BLACK : WHITE;
    }
  }
  hist_ = hist;
}

std::unordered_map<int, int> Histogram::getPeriodicity() {
  std::unordered_map<int, int> map;
  for (int i = 0; i < 256; i++)
    if (intensities_[i])
      map.insert(std::pair<int, int>(i, intensities_[i]));
  return map;
}
