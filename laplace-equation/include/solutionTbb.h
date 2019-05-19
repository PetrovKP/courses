//
// Created by kirill on 29.03.17.
//

#ifndef LAPLACE_EQUATION__TBB_H
#define LAPLACE_EQUATION__TBB_H

#include "tbb/tbb.h"
#include "serial.h"

int parallelTBB(Setting *setting, double *u);

#endif //LAPLACE_EQUATION__TBB_H
