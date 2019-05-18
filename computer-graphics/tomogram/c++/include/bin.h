//
// Created by kirill on 26.10.16.
//

#ifndef TOMOGRAM_BIN_H
#define TOMOGRAM_BIN_H

#include <string>
#include <vector>

typedef std::vector<short> array;

namespace bin {
void readBin(const std::string &path, array &arr, int &Y, int &X, int &Z);
}

#endif //TOMOGRAM_BIN_H
