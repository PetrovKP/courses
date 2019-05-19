//
// Created by kirill on 13.03.17.
//


#include <mpi.h>
#include <omp.h>
#include <math.h>
//#include "tbb/tbb.h"

#include "serial.h"
#include "parallel.h"
//#include "solutionTbb.h"

//#define OFF_ENABLE_SERIAL
//#define OFF_ENABLE_PARALLEL_OMP
#define OFF_ENABLE_PARALLEL_TBB

#define ROOT 0

#define PATH_INIT_SETTING "../../initial/setting.ini"
//#define PATH_INIT_SETTING "C:\\Users\\nesol\\Desktop\\tmp\\laplace-equation\\initial\\setting.ini"
//#define PATH_INIT_SETTING "setting.ini"

#define PATH_SERIAL_RESULT "../../result/SerialResult.txt"
//#define PATH_SERIAL_RESULT "C:\\Users\\nesol\\Desktop\\tmp\\laplace-equation\\result\\SerialResult.txt"
#define PATH_PARALLEL_RESULT "../../result/ParallelResult.txt"
//#define PATH_PARALLEL_RESULT "C:\\Users\\nesol\\Desktop\\tmp\\laplace-equation\\result\\ParallelResult.txt"

int main(int argc, char **argv) {
  int gridSize, Nr, N;
  int sizeP, rankP;
  float EPS;
  MPI_Status status;
  Setting setting;
  float* u = NULL;
  double t0 = 0.0, t1 = 0.0;
  double diffTimeSerial = 0.0, diffTimeParallel, boost;
  int count;

  float dx, dy;
  float xs, ys;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &sizeP);
  MPI_Comm_rank(MPI_COMM_WORLD, &rankP);

  if (rankP == ROOT) {
    readSetting(PATH_INIT_SETTING, &setting);
    N = setting.DIM + 2;
    printf("N =\t%d\n", N);
    EPS = setting.EPS;
    u = new float[N*N];
    clear(u, N);

// ЗАПОЛНЕНИЕ ГРАНИЧНЫХ УСЛОВИЙ
//  Шаг по xs и ys одинаковый
    dx = fabsf(setting.XEND - setting.XSTART)/(N - 1);
    dy = fabsf(setting.YEND - setting.YSTART)/(N - 1);
    xs = setting.XSTART;
    ys = setting.YSTART;
    feel(N, u, xs, ys, dx, dy, 0, 0);

// ****************************

    #ifndef OFF_ENABLE_SERIAL
    t0 = omp_get_wtime();
    count = consistentSolution(&setting, u);
    t1 = omp_get_wtime();

    diffTimeSerial = t1 - t0;
    printf("Count iteration\t%d\n", count);
    printf("Time Serial\t%lf\n", diffTimeSerial);
    writeFunction(PATH_SERIAL_RESULT, u, N, setting.DIM, 0, 0);
    #endif
    clear(u, N);
  }
  MPI_Bcast(&N, 1, MPI_INT, ROOT, MPI_COMM_WORLD);
  MPI_Bcast(&EPS, 1, MPI_FLOAT, ROOT, MPI_COMM_WORLD);
  MPI_Bcast(&dx, 1, MPI_FLOAT, ROOT, MPI_COMM_WORLD);
  MPI_Bcast(&dy, 1, MPI_FLOAT, ROOT, MPI_COMM_WORLD);
  MPI_Bcast(&xs, 1, MPI_FLOAT, ROOT, MPI_COMM_WORLD);
  MPI_Bcast(&ys, 1, MPI_FLOAT, ROOT, MPI_COMM_WORLD);

  gridSize = (int) sqrt(sizeP);
  Nr = (N-2)/gridSize + 2;
  float *ur = new float[Nr * Nr];
  memset(ur, 0, sizeof(float)*Nr*Nr);

  MPI_Comm gridComm;  	// Коммуникатор в виде квадратной решетки

  const int DIM_CART = 2;
//  размер каждой размерности
  int dims[DIM_CART];   int periods[DIM_CART];
  int gridCoords[DIM_CART], cartrank;

  dims[0] = gridSize; dims[1] = gridSize;
//  наличие циклов в каждой размерности
  periods[0] = 0; periods[1] = 0;
//  разрешение системе менять номера процессов
  int reorder = 0;
  MPI_Cart_create(MPI_COMM_WORLD, DIM_CART, dims, periods, reorder, &gridComm);

  // Координаты текущего процесса в процессной решетке и ранг в решётке
  MPI_Comm_rank(gridComm, &cartrank);
  // Определение координат процесса в решетке
  MPI_Cart_coords(gridComm, rankP, DIM_CART, gridCoords);

  //  SCATTER
  feel(Nr, ur, xs, ys, dx, dy, (Nr-2)*gridCoords[1], (Nr-2)*gridCoords[0]);

  #ifndef OFF_ENABLE_PARALLEL_OMP
  omp_set_num_threads(1);
  if (rankP == ROOT) {
    t0 = MPI_Wtime();
    printf("started\n");
  }
  // PARALLEL MPI+OMP
  count = parallelMPI(ur, Nr, EPS, gridComm);
  if (rankP == ROOT) t1 = MPI_Wtime();

  if (rankP == ROOT) {
    printf("finish\n");
    diffTimeParallel = t1 - t0;
    printf("Count iteration\t%d\n", count);
    printf("Time Parallel MPI+OMP\t%lf\n", diffTimeParallel);

    //  Ускорение
    boost = diffTimeSerial / diffTimeParallel;
    printf("Boost\t%lf\n", boost);
  }
  #endif
  //  GATHER!
  gather_by_block(u, ur, N, Nr, gridComm);
  if (rankP == ROOT) {
    writeFunction(PATH_PARALLEL_RESULT, u, N, setting.DIM, 0, 0);
    delete[] u;
  }

  delete[] ur;

  MPI_Finalize();
  return 0;
}
