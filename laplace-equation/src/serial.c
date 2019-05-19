//
// Created by kirill on 29.03.17.
//
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "serial.h"

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

int readSetting(const char *path, Setting *setting) {
  FILE *fp;
  if ( !(fp = fopen(path, "r")) )
    return NO_FILE;

  if ( !fscanf(fp, "DIM=%d\n", &setting->DIM) ) return READING_ERROR;
  if ( !fscanf(fp, "EPS=%e\n", &setting->EPS) ) return READING_ERROR;
  if ( !fscanf(fp, "XSTART=%f\n", &setting->XSTART) ) return READING_ERROR;
  if ( !fscanf(fp, "XEND=%f\n", &setting->XEND) ) return READING_ERROR;
  if ( !fscanf(fp, "YSTART=%f\n", &setting->YSTART) ) return READING_ERROR;
  if ( !fscanf(fp, "YEND=%f\n", &setting->YEND) ) return READING_ERROR;

  return OK;
}

int readFunction(const char* path, float** function, int dim, int NX) {
  FILE *fp;
  if ( !(fp = fopen(path, "r")) )
    return NO_FILE;

  for (int i = 0; i < dim; i++)
    if (i%(NX + 2)!=0 && i%(NX + 2) != NX + 1 )
      if ( !fscanf(fp, "%f\n", (*function) + i) )
        return READING_ERROR;

  return OK;
}

void writeFunction(const char* path, float* function, int dim, int count, int startX, int startY) {
  FILE *fp;
  fp = fopen(path, "w");

  for (int i = startX+1; i < startX+count+1; i++) {
    for (int j = startY+1; j < startY+count+1; j++)
      fprintf(fp, "%.7e ", function[i*(dim) + j]);
    fprintf(fp, "\n");
  }

  fclose(fp);
}

int consistentSolution(Setting *setting, float *u) {
  int iterCount = 0;
  float dmax, temp, dm, *dmaxs;
  int N = setting->DIM + 2;
  dmaxs = (float*)calloc(N,sizeof(float));
  do {
    dmax = 0.f; // максимальное изменение значений u
    for (int i = 1; i < N - 1; i++) {
      for (int j = 1; j < N - 1; j++) {
        temp = u[i*N + j];
        u[i*N + j] = 0.25f*(u[(i - 1)*N + j] + u[(i + 1)*N + j] +
            u[i*N + j - 1] + u[i*N + j + 1]);
        dm = fabsf(temp - u[i*N + j]);
        dmaxs[j] = dm;
      }
      for (int j = 1; j < N - 1; j++) {
        if (dmax < dmaxs[j])
          dmax = dmaxs[j];
      }
    }

    iterCount++;
  } while (dmax > setting->EPS);
  free(dmaxs);
  return iterCount;
}

void clear(float *u, int N) {
  for (int i = 1; i < N - 1; i++) {
    for (int j = 1; j < N - 1; j++)
      u[i * N + j] = 0.0;
  }
}


void unpack(float* in, float* ur, int NR, int which) {
  switch (which) {
    case LEFT: {
      for (int y = 0; y < NR; y++)
        ur[y*NR + 0] = in[y];
      break;
    }
    case RIGHT: {
      for (int y = 0; y < NR; y++)
        ur[y*NR + NR-1] = in[y];
      break;
    }
    case TOP: {
      for (int x = 0; x < NR; x++)
        ur[(NR - 1)*NR + x] = in[x];
      break;
    }
    case DOWN: {
      for (int x = 0; x < NR; x++)
        ur[0*NR + x] = in[x];
      break;
    }
  }
}

// Нижняя граница Y
float yleft(float y) {
  return 0.0;
}
// Верхняя граница Y
float yright(float y) {
  return 0.0;
}

// Нижняя граница X
float xdown(float x) {
  return sinf(M_PI*x);

}

// Верхняя граница X
float xtop(float x) {
  return sinf(M_PI*x)*expf(-M_PI);
}

void feel(int N, float* u, float xs, float ys, float dx, float dy, int shiftX, int shiftY) {
  float *bufferTop = (float*)calloc(N,sizeof(float));
  float *bufferDown = (float*)calloc(N,sizeof(float));
  float *bufferLeft = (float*)calloc(N,sizeof(float));
  float *bufferRight = (float*)calloc(N,sizeof(float));

  for (int i = 0; i < N; i++) {
    bufferTop[i] = xtop(xs + (i+shiftX)*dx);
    bufferDown[i] = xdown(xs + (i+shiftX)*dx);

    bufferLeft[i] = yleft(ys + (i+shiftY)*dy);
    bufferRight[i] = yright(ys + (i+shiftY)*dy);
  }

  unpack(bufferDown, u, N, DOWN);
  unpack(bufferTop, u, N, TOP);
  unpack(bufferLeft, u, N, LEFT);
  unpack(bufferRight, u, N, RIGHT);

  free(bufferTop);
  free(bufferDown);
  free(bufferLeft);
  free(bufferRight);
}
