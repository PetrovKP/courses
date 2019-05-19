#ifndef IMAGE_PROCESSING_SEGMENTATION_H
#define IMAGE_PROCESSING_SEGMENTATION_H

#include <opencv2/imgproc/imgproc.hpp>

#include <set>
#include <vector>

namespace Segmentation {

static cv::Size size;

struct region {
  // tree data structure
  std::vector<region> childs;
  bool validity;

  // tree for split&merge procedure
  cv::Rect roi;
  cv::Mat m;
  cv::Scalar label;
  cv::Mat mask;

  int area;
  int intesity;
};


struct value {
  int area;
  int intesity;
};


bool predicate(const cv::Mat& src);

bool mergeTwoRegion(region &parent, const cv::Mat &src, region &r1, region &r2);
void merge(const cv::Mat& src, region& r);

region split(const cv::Mat& src, cv::Rect roi);

void draw_region(cv::Mat& img, region r);

void print_region(region r);

}

#endif //IMAGE_PROCESSING_SEGMENTATION_H
