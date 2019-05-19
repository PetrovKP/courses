#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <fstream>

#include "huffman.h"

int main() {
  Huffman huffman;

  cv::Mat img = cv::imread("../../../images/debug.bmp",CV_8U);
//  cv::Mat img = cv::imread("../../../images/lena.bmp", CV_8U);
//  cv::Mat img = cv::imread("../../../images/ava.bmp",CV_8U);

  std::ofstream outfile("../../../bin/debug.bin", std::ios::out | std::ios::binary);
//  std::ofstream outfile("../../../bin/lena.bin", std::ios::out | std::ios::binary);
//  std::ofstream outfile("../../../bin/ava.bin", std::ios::out | std::ios::binary);
  huffman.encoder(img, outfile);
  outfile.close();

  std::ifstream infile("../../../bin/debug.bin", std::ios::in | std::ios::binary);
//  std::ifstream infile("../../../bin/lena.bin", std::ios::in | std::ios::binary);
//  std::ifstream infile("../../../bin/lena.bin", std::ios::in | std::ios::binary);
  cv::Mat res = huffman.decoder(infile);
  cv::imwrite("../../../bin/test.bmp", res);

  std::cout << "Степень сжатия - " << huffman.getCompressionRatio();

  return 0;
}
