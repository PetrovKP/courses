//
// Created by kirill on 29.03.17.
//

#ifndef LAPLACE_EQUATION__OMP_H
#define LAPLACE_EQUATION__OMP_H

#ifdef __cplusplus
extern "C" {
#endif

#define DIM_GRIDS 2

#include "serial.h"

int parallelMPI(float* u, int n, float EPS, MPI_Comm comm);
float parallelOMP(float* u, int NR, float* dmaxs);
void gather_by_block(float *u, float *u_chank, int N, int N_chank, MPI_Comm gridComm);


#ifdef __cplusplus
}
#endif

#endif //LAPLACE_EQUATION__OMP_H
