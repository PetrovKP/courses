//
// Created by kirill on 26.10.16.
//
#include "bin.h"
#include <fstream>

using std::ifstream;

void bin::readBin(const std::string &path, array &arr, int &Y, int &X, int &Z) {
  ifstream data(path, std::ios::binary);

  data.read(reinterpret_cast<char *>(&X), sizeof(X));
  data.read(reinterpret_cast<char *>(&Y), sizeof(Y));
  data.read(reinterpret_cast<char *>(&Z), sizeof(Z));

  size_t sizeArray = static_cast<size_t >(X * Y * Z) ;

  arr.reserve(sizeArray);
  // Считывание массива! магия, не трогать!
  data.read(reinterpret_cast<char *>(&arr[0]), sizeArray*sizeof(short));
  data.close();
}