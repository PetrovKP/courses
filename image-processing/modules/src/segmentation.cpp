//
// Created by kirill on 14.04.17.
//

#include <segmentation.h>
#include <iostream>
#include <array>

using namespace Segmentation;

bool Segmentation::predicate(const cv::Mat& src) {
  cv::Scalar stddev, mean;
  cv::meanStdDev(src, mean, stddev);
  int Targ = 120;
  bool result = true;
  if (src.rows*src.cols <= 100)
    result = true;
  else {
    for (int i = 0; i < src.rows; i++)
      for (int j = 0; j < src.cols; j++)
        if (fabs(src.at<cv::Vec3b>(i, j)[0] - mean[0]) > Targ ||
            fabs(src.at<cv::Vec3b>(i, j)[1] - mean[1]) > Targ ||
            fabs(src.at<cv::Vec3b>(i, j)[2] - mean[2]) > Targ) {
          result = false;
          break;
        }
//    result = (stddev[0]<5.6);
  }
  return result;
}

bool Segmentation::mergeTwoRegion(region &parent, const cv::Mat &src, region &r1, region &r2) {
  if(r1.childs.size()==0 && r2.childs.size()==0) {

    cv::Rect roi1 = r1.roi;
    cv::Rect roi2 = r2.roi;
    cv::Rect roi12 = roi1 | roi2;
    if(predicate( src(roi12) )) {
      r1.roi = roi12;
      r1.area = roi1.area() + roi2.area();

      r1.mask = cv::Mat::zeros(size, CV_8U);
      cv::rectangle(r1.mask, r1.roi, 1, CV_FILLED);

      r2.validity = false;
      return true;
    }
  }
  return false;
}

void Segmentation::merge(const cv::Mat& src, region& r) {

  bool row1=false, row2=false, col1=false, col2=false;

  if(r.childs.size()<1) return;

  // try with the row
  row1 = mergeTwoRegion(r, src, r.childs[0], r.childs[1]);
  row2 = mergeTwoRegion(r, src, r.childs[2], r.childs[3]);

  if( !(row1 | row2) ) {
    // try with column
    col1 = mergeTwoRegion(r, src, r.childs[0], r.childs[2]);
    col2 = mergeTwoRegion(r, src, r.childs[1], r.childs[3]);
  }

  for(int i=0; i<r.childs.size(); i++) {
    if(r.childs[i].childs.size()>0)
      merge(src, r.childs[i]);
  }
}

region Segmentation::split(const cv::Mat& src, cv::Rect roi) {
  std::vector<region> childs;
  region r;

  r.roi = roi;
  r.m = src;
  r.mask = cv::Mat::zeros(size, CV_8U);
  rectangle(r.mask, r.roi, 1, CV_FILLED);
  r.validity = true;

  if(predicate(src)) {
    cv::Scalar mean, s;
    cv::meanStdDev(src, mean, s);
    r.label = mean;
  } else {
    int w = src.cols/2;
    int h = src.rows/2;
    region r1 = split(src(cv::Rect(0,0, w,h)), cv::Rect(roi.x, roi.y, w,h));
    region r2 = split(src(cv::Rect(w,0, w,h)), cv::Rect(roi.x+w, roi.y, w,h));
    region r3 = split(src(cv::Rect(0,h, w,h)), cv::Rect(roi.x, roi.y+h, w,h));
    region r4 = split(src(cv::Rect(w,h, w,h)), cv::Rect(roi.x+w, roi.y+h, w,h));
    r.childs.push_back( r1 );
    r.childs.push_back( r2 );
    r.childs.push_back( r3 );
    r.childs.push_back( r4 );
  }
  return r;
}

void Segmentation::draw_region(cv::Mat& img, region r) {
  if(r.validity && r.childs.size()==0)
    cv::rectangle(img, r.roi, r.label, CV_FILLED);
  for(int i=0; i<r.childs.size(); i++) {
    draw_region(img, r.childs[i]);
  }
}

std::array<double, 2> centerOfMass(cv::Rect roi, cv::Scalar intensity, double A) {
  double intensR =(intensity.val[0]+intensity.val[1]+intensity.val[2])/3.0;
  double resultX = 0., resultY = 0.;
  for (int x = 0; x < roi.width; x++)
    for (int y = 0; y < roi.height; y++) {
      resultX += x*intensR/A;
      resultY += y*intensR/A;
    }
  return {resultX, resultY};
}

double moment(cv::Rect roi, cv::Scalar intensity) {
  double result = 0.;
  double intensR =(intensity.val[0]+intensity.val[1]+intensity.val[2])/3;

  for (int x = 0; x < roi.width; x++)
    for (int y = 0; y < roi.height; y++) {
      result+=x*y*intensR;
    }
  return result;
}


void Segmentation::print_region(region r) {
  if(r.validity && r.childs.size()==0) {
    std::cout << "x - "<< r.roi.x << " y - " << r.roi.y << std::endl;
    std::cout << "area - " << r.roi.area() << std::endl;
    std::array<double, 2> center = centerOfMass(r.roi, r.label, r.roi.area());
    std::cout << "center Of mass x - " << center[0]<< " y - " << center[1] << std::endl;
    std::cout << "moment - " << moment(r.roi, r.label) << std::endl;
    std::cout << "intensity - " << r.label << std::endl;
    std::cout << "---" << std::endl;
  }
  for(int i=0; i<r.childs.size(); i++) {
    print_region(r.childs[i]);
  }
}