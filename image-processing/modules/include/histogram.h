
#ifndef IMAGE_PROCESSING_HISTOGRAM_H
#define IMAGE_PROCESSING_HISTOGRAM_H

#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <map>
#include <algorithm>
#include <unordered_map>

class Histogram {
 public:
  Histogram(const cv::Mat &mat);

  std::unordered_map<int, int> getPeriodicity();

  int maxValBin() const;
  int minValBin() const;
  int maxValIntensity() const;
  int minValIntensity() const;

//  Сглаживание гистограммы
  void smooth();
//  Отрисовка гистограммы
  void showHist(const std::string &name);

  int at(int val) const;
  void printIntensities() const;
 private:
  static const uchar BLACK;
  static const uchar WHITE;
  cv::Mat mat_;
  cv::Mat hist_;
  int intensities_[256];
  int maxIntensity_;
  cv::Mat generateHist(const cv::Mat &mat);
  int calcNewValue(int value, int max) const;
};

#endif //IMAGE_PROCESSING_HISTOGRAM_H
