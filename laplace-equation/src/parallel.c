//
// Created by kirill on 29.03.17.
//

#include <math.h>
#include <stdlib.h>
#include <omp.h>
#include <mpi.h>

#include "parallel.h"
#include <stdio.h>

float parallelOMP(float* u, int NR, float* dmaxs) {
  int i, j, nx;
  float d, temp;

  float dmax = 0.0; // максимальное изменение значений u
  // нарастание волны (nx – размер волны)
//  omp_lock_t dmax_lock;
  //omp_init_lock(&dmax_lock);
  for (nx = 1; nx < NR - 1; nx++) {
    dmaxs[nx] = 0;
#pragma omp parallel for private(i, j, temp, d)
    for (i = 1; i < nx + 1; i++) {
      j = nx + 1 - i;
      temp = u[i * NR + j];
      u[i * NR + j] = 0.25f * (u[(i - 1) * NR + j] + u[(i + 1) * NR + j] +
          u[i * NR + j - 1] + u[i * NR + j + 1]);
      d = fabsf(temp - u[i * NR + j]);
      if (dmaxs[i] < d) dmaxs[i] = d;
    } // конец параллельной области
  }
  // затухание волны
  for (nx = NR - 3; nx > 0; nx--) {
#pragma omp parallel for private(i, j, temp, d)
    for (i = NR - nx - 1; i < NR - 1; i++) {
      j = 2 * (NR - 2) - nx - i + 1;
      temp = u[i * NR + j];
      u[i * NR + j] = 0.25f * (u[(i - 1) * NR + j] + u[(i + 1) * NR + j] +
          u[i * NR + j - 1] + u[i * NR + j + 1]);
      d = fabsf(temp - u[i * NR + j]);
      if (dmaxs[i] < d) dmaxs[i] = d;
    }
  } // конец параллельной области

  
 #pragma omp parallel for reduction(max: dmax) private(i)
//#pragma omp parallel for private(i)
  for (i = 1; i < NR - 1; i++) {
//	 omp_set_lock(&dmax_lock);
    if (dmax < dmaxs[i])
      dmax = dmaxs[i];
//	omp_unset_lock(&dmax_lock);
  }
  return dmax;
}


int parallelMPI(float* u, int n, float EPS, MPI_Comm comm) {
  float dmaxloc, dmaxglob;
  int count = 0;
  MPI_Status status[8];
  MPI_Request request[8];

  int dims[DIM_GRIDS], periods[DIM_GRIDS], gridCoords[DIM_GRIDS];

  MPI_Cart_get(comm, DIM_GRIDS, dims, periods, gridCoords);

  float *dmaxs = malloc(n*sizeof(float));

  MPI_Datatype col;
  MPI_Type_vector(n, 1, n, MPI_FLOAT, &col);
  MPI_Type_commit(&col);

  MPI_Datatype row;
  MPI_Type_contiguous(n, MPI_FLOAT, &row);
  MPI_Type_commit(&row);

  int rank_down, rank_top, rank_left, rank_right;

  int rank;
  MPI_Cart_rank(comm, gridCoords, &rank);
  MPI_Cart_shift(comm, 1, 1, &rank_left, &rank_right);
  MPI_Cart_shift(comm, 0, 1, &rank_down, &rank_top);

 // printf("rank - %d; left %d; right % d; top %d; down %d\n", rank, rank_left, rank_right, rank_top, rank_down);

  MPI_Send(&u[(n-2)*n + 0], 1, row, rank_top, 2, comm);
  MPI_Send(&u[0*n + 1], 1, col, rank_left, 3, comm);

  do {
    MPI_Recv(&u[(n-1)*n + 0], 1, row, rank_top, 0, comm, &status[0]);
    MPI_Recv(&u[0*n + 0], 1, col, rank_left, 1, comm, &status[1]);
    MPI_Recv(&u[0*n + 0], 1, row, rank_down, 2, comm, &status[2]);
    MPI_Recv(&u[0*n + n-1], 1, col, rank_right, 3, comm, &status[3]);

    count++;
//  Обработка блока с оценкой
    dmaxloc = parallelOMP(u, n, dmaxs);

    MPI_Isend(&u[1*n + 0], 1, row, rank_down, 0, comm, &request[0]);
    MPI_Isend(&u[0*n + n-2], 1, col, rank_right, 1, comm, &request[1]);
    MPI_Isend(&u[(n-2)*n + 0], 1, row, rank_top, 2, comm, &request[2]);
    MPI_Isend(&u[0*n + 1], 1, col, rank_left, 3, comm, &request[3]);

    MPI_Allreduce(&dmaxloc, &dmaxglob, 1, MPI_FLOAT, MPI_MAX, comm);
  } while ( dmaxglob > EPS );
  free(dmaxs);

  MPI_Type_free(&row);
  MPI_Type_free(&col);

  return count;
}

void gather_by_block(float *u, float *u_chank, int N, int N_chank, MPI_Comm gridComm) {

  int dims[DIM_GRIDS], periods[DIM_GRIDS], gridCoords[DIM_GRIDS];

  MPI_Cart_get(gridComm, DIM_GRIDS, dims, periods, gridCoords);
  MPI_Comm colComm, rowComm;

  int remain_dims[DIM_GRIDS];
  remain_dims[0] = 1; remain_dims[1] = 0;
  MPI_Cart_sub(gridComm, remain_dims, &colComm);

  remain_dims[0] = 0; remain_dims[1] = 1;
  MPI_Cart_sub(gridComm, remain_dims, &rowComm);

  int nr = N_chank - 2;
  float *ur = (float *)malloc(nr*nr*sizeof(float));

  for (int i = 1; i < N_chank-1; i++) {
    for (int j = 1; j < N_chank-1; j++) {
      ur[(i-1)*nr+j-1]=u_chank[i*N_chank + j];
    }
  }

  float* u_row = NULL;

  if (gridCoords[1] == 0) {
    u_row = (float *)malloc((nr)*N*sizeof(float));;
  }

  for (int i = 0; i < nr; i++)
    MPI_Gather(ur + i*nr, nr, MPI_FLOAT, u_row + (i)*N, nr, MPI_FLOAT, 0, rowComm);

  if (gridCoords[1] == 0) {
    MPI_Gather(u_row, N * (nr), MPI_FLOAT, u+N+1 , N * nr, MPI_FLOAT, 0, colComm);
    free(u_row);
  }
  free(ur);
}
