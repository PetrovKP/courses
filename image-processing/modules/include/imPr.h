
#ifndef IMAGE_PROCESSING_IMPR_H
#define IMAGE_PROCESSING_IMPR_H

#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace imPr {

static const cv::Vec3f BLACK = cv::Vec3f(0.f, 0.f, 0.f);
static const cv::Vec3f WHITE = cv::Vec3f(255.f, 255.f, 255.f);

// Перевод в серый цвет по среднему
cv::Mat avarangeConvertGray(cv::Mat mat);

// Перевод в серый по стандартной формуле
cv::Mat stdConvertGray(cv::Mat mat);

// Перевод серого в чёрно-белый
cv::Mat binarization(const cv::Mat &mat, float T);

double mean(const cv::Mat &mat1, const cv::Mat &mat2);

// Вычисление расстояния между изображениями
double difference(const cv::Mat &mat1, const cv::Mat &mat2);

void diffImage(const cv::Mat &mat1, const cv::Mat &mat2);

// Шум
cv::Mat impulseNoise(const cv::Mat &mat, double noisePart, const cv::Vec3b &color);

// Фильтрация
cv::Mat localFiltration(const cv::Mat &mat, int r);

double SSIM ( cv::Mat source, cv::Mat noise);

};

#endif // IMAGE_PROCESSING_IMPR_H
