
#include <algorithm>
#include <iostream>
#include "imPr.h"

void generateRand(int *vec, const int dim, const int size) {
  srand((unsigned int) time(NULL));
  int in, im;
  im = 0;

  for (in = 0; in < dim && im < size; ++in) {
    int rn = dim - in;
    int rm = size - im;
    if (rand() % rn < rm)
      vec[im++] = in + 1;
  }
  assert(im == size);
}


cv::Mat imPr::avarangeConvertGray(cv::Mat mat) {
  cv::Vec3f intensity;
  float blue, green, red;
  float gray;
  for (int i = 0; i < mat.rows; i++) {
    for (int j = 0; j < mat.cols; j++) {
      intensity = mat.at<cv::Vec3b>(i, j);

      blue = intensity.val[0];
      green = intensity.val[1];
      red = intensity.val[2];

      gray = (blue + green + red) / 3.f;

      mat.at<cv::Vec3b>(i, j) = cv::Vec3f(gray, gray, gray);
    }
  }
  return mat;
}

cv::Mat imPr::stdConvertGray(cv::Mat mat) {
  cv::Vec3f intensity;
  float blue, green, red;
  float gray;
  for (int i = 0; i < mat.rows; i++) {
    for (int j = 0; j < mat.cols; j++) {
      intensity = mat.at<cv::Vec3b>(i, j);

      blue = intensity.val[0];
      green = intensity.val[1];
      red = intensity.val[2];

      gray = blue * 0.114f + red * 0.299f + green * 0.587f;

      mat.at<cv::Vec3b>(i, j) = cv::Vec3f(gray, gray, gray);
    }
  }
  return mat;
}

cv::Mat imPr::binarization(const cv::Mat &mat, float T) {
  cv::Mat result = mat;

  float intensity;

  for (int i = 0; i < mat.rows; i++) {
    for (int j = 0; j < mat.cols; j++) {
      intensity = mat.at<cv::Vec3b>(i, j).val[0];

      result.at<cv::Vec3b>(i, j) = (intensity > T) ? imPr::WHITE : imPr::BLACK;
    }
  }
  return result;
}

double imPr::difference(const cv::Mat &mat1, const cv::Mat &mat2) {
  int rows = std::min(mat1.rows, mat2.rows);
  int cols = std::min(mat1.cols, mat2.cols);

  double mean = imPr::mean(mat1, mat2);

  double variance = 0.f;

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      double s1 = mat1.at<cv::Vec3b>(i, j).val[0];
      double s2 = mat2.at<cv::Vec3b>(i, j).val[0];
      variance += (fabs(s1 - s2) - mean) * (fabs(s1 - s2) - mean);
    }
  }
  variance /= (rows * cols);
  variance = sqrt(variance);
  return variance;
}

double imPr::mean(const cv::Mat &mat1, const cv::Mat &mat2) {
  int rows = std::min(mat1.rows, mat2.rows);
  int cols = std::min(mat1.cols, mat2.cols);
  double mean = 0.f;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      double s1 = mat1.at<cv::Vec3b>(i, j).val[0];
      double s2 = mat2.at<cv::Vec3b>(i, j).val[0];
      mean += fabs(s1 - s2);
    }
  }
  mean /= (rows * cols);
  return mean;
}

void imPr::diffImage(const cv::Mat &mat1, const cv::Mat &mat2) {
  cv::Vec3b WHITE = cv::Vec3b(255, 255, 255);
  cv::Vec3b BLUE = cv::Vec3b(255, 0, 0);
  cv::Vec3b GREEN = cv::Vec3b(0, 255, 0);

  cv::Mat diffImg = mat1.clone();

  for (int i = 0; i < diffImg.rows; i++) {
    for (int j = 0; j < diffImg.cols; j++) {
      if (mat1.at<cv::Vec3b>(i, j) != mat2.at<cv::Vec3b>(i, j)) {
        diffImg.at<cv::Vec3b>(i, j) = (mat1.at<cv::Vec3b>(i, j) == WHITE) ? BLUE : GREEN;
      } else {
        diffImg.at<cv::Vec3b>(i, j) = mat1.at<cv::Vec3b>(i, j);
      }
    }
  }

  cv::imshow("Image difference", diffImg);
}

cv::Mat imPr::localFiltration(const cv::Mat &mat, int r) {
  //cv::Mat newMat = cv::Mat(mat.rows, mat.cols, mat.type());
  cv::Mat newMat = mat.clone();
  for (int i = 0; i < mat.rows; i++) {
    for (int j = 0; j < mat.cols; j++) {
      int Imax1 = 0;
      int Imax2 = 0;
      int Imax3 = 0;

      int Imin1 = 255;
      int Imin2 = 255;
      int Imin3 = 255;

      for (int l = -r; l <= r; l++) {
        for (int k = -r; k <= r; k++) {

          if (((i + l) < 0) || ((j + k) < 0)) {
            break;
          }

          int intensity = mat.at<cv::Vec3b>(i + l, j + k)[0];
          if (intensity > Imax1) {
            Imax1 = intensity;
          }
          if (intensity < Imin1) {
            Imin1 = intensity;
          }

          intensity = mat.at<cv::Vec3b>(i + l, j + k)[1];
          if (intensity > Imax2) {
            Imax2 = intensity;
          }
          if (intensity < Imin2) {
            Imin2 = intensity;
          }

          intensity = mat.at<cv::Vec3b>(i + l, j + k)[2];
          if (intensity > Imax3) {
            Imax3 = intensity;
          }
          if (intensity < Imin3) {
            Imin3 = intensity;
          }
        }
      }

      int newIntensity1 = (Imax1 + Imin1) / 2;
      int newIntensity2 = (Imax2 + Imin2) / 2;
      int newIntensity3 = (Imax3 + Imin3) / 2;
      newMat.at<cv::Vec3b>(i, j) = cv::Vec3b(newIntensity1, newIntensity2, newIntensity3);
    }
  }
  return newMat;
}

cv::Mat imPr::impulseNoise(const cv::Mat &mat, double noisePart, const cv::Vec3b &color) {
  cv::Mat result = mat.clone();
  int dim = result.rows*result.cols;
  int sizePart = (int)(noisePart*dim);
  int* indexs = new int [sizePart];
  generateRand(indexs, dim, sizePart);
  for (int i = 0; i < sizePart; i++)
    result.at<cv::Vec3b>(indexs[i]/result.rows, indexs[i]%result.cols) = color;

  delete[] indexs;
  return result;
}

const double C = 1e-12;

double imPr::SSIM ( cv::Mat source, cv::Mat noise)
{
  double av_source = 0, av_noise = 0;
  double sig_s = 0.0, sig_n = 0.0, sig_sn = 0.0;
  for(int i = 0; i < source.rows; ++i)
    for(int j = 0; j <source.cols; ++j)
      av_source+=source.at<uchar>(i,j);
  av_source /= (source.rows * source.cols);

  for(int i = 0; i < noise.rows; ++i)
    for(int j = 0; j <noise.cols; ++j)
      av_noise+=noise.at<uchar>(i,j);
  av_noise /= (noise.rows * noise.cols);

  for(int i = 0; i < source.rows; ++i)
    for(int j = 0; j <source.cols; ++j)
    {

      sig_s += pow((double)(source.at<uchar>(i,j) - av_source),2);
      sig_n += pow((double)(noise.at<uchar>(i,j) - av_noise),2);
      sig_sn +=(source.at<uchar>(i,j) - av_source)*(noise.at<uchar>(i,j) - av_noise);
    }
  sig_s /= (source.rows * source.cols - 1);
  sig_s = sqrt(sig_s);

  sig_n /= (noise.rows * noise.cols - 1);
  sig_n = sqrt(sig_n);

  sig_sn /= (source.rows * source.cols - 1);

  double l,c,s;

  l = (2*av_source*av_noise+C)/(av_source*av_source + av_noise*av_noise + C);
  c = (2*sig_s*sig_n + C)/(sig_s*sig_s + sig_n*sig_n+C);
  s = (sig_sn + C) / (sig_s*sig_n +C);

  return l*c*s;
}
