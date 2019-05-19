//
// Created by kirill on 29.03.17.
//

#ifndef LAPLACE_EQUATION__SERIAL_H
#define LAPLACE_EQUATION__SERIAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <wchar.h>
#include <math.h>

#define OK 0
#define NO_FILE 100
#define READING_ERROR 101

typedef struct {
  int DIM;
  float EPS;
  float XSTART;
  float XEND;
  float YSTART;
  float YEND;
} Setting;

typedef enum {
  RIGHT,
  LEFT,
  TOP,
  DOWN,
} op;

// Функции для чтения/записи настроек/функции
int readSetting(const char *path, Setting *setting);
int readFunction(const char* path, float** function, int dim, int NX);
void writeFunction(const char* path, float* function, int dim, int count, int startX, int startY);

void clear(float *u, int N);

void feel(int N, float* u, float xs, float ys, float dx, float dy, int shiftX, int shiftY);

// Последовательная версия
int consistentSolution(Setting *setting, float *u);

#ifdef __cplusplus
}
#endif

#endif //LAPLACE_EQUATION__SERIAL_H
