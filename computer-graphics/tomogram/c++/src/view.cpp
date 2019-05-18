  //
// Created by kirill on 26.10.16.
//

#include <iostream>
#include <GL/glut.h>

#include "view.h"

template <class type>
inline type clamp(type value, type min, type max) {
  if (value < min)
    return min;
  else if (value > max)
    return max;
  else
    return value;
}

inline float transfer_function(short value) {
  float intensity = 255.f*(value - View::min)/(View::max - View::min);
  float newValue = clamp<float >(intensity, 0.f, 255.f);
  return newValue;
}

void View::setup() {
  currentLayer = 0;
  bin::readBin("./../data/testdata.bin", arr, X, Y, Z);
  glShadeModel(GL_SMOOTH);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, X, 0, Y, -1, 1);
  glViewport(0, 0, width, heigth);
}

void View::display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  View::render();

  glutSwapBuffers();
}

void View::drawQuads(int layerNumber) {
  glBegin(GL_QUADS);
  short value;
  float color;
  for(int x_color = 0; x_color < X - 1; x_color++)
    for(int y_color = 0; y_color < Y - 1; y_color++) {

      //--------------------------------------------------------------------
      //                      1 вершина
      //--------------------------------------------------------------------

      value = arr[x_color + y_color * X + layerNumber * X * Y];

      color = transfer_function(value) / 255.f;
      glColor3f(color, color, color);
      glVertex2f(x_color, y_color);

      //--------------------------------------------------------------------
      //                      2 вершина
      //--------------------------------------------------------------------

      value = arr[x_color + (y_color + 1) * X + layerNumber * X * Y];

      color = transfer_function(value) / 255.f;
      glColor3f(color, color, color);
      glVertex2f(x_color, y_color + 1);

      //--------------------------------------------------------------------
      //                      3 вершина
      //--------------------------------------------------------------------

      value = arr[x_color + 1 + (y_color + 1) * X + layerNumber * X * Y];

      color = transfer_function(value) / 255.f;
      glColor3f(color, color, color);
      glVertex2f(x_color + 1, y_color + 1);

      //--------------------------------------------------------------------
      //                      4 вершина
      //--------------------------------------------------------------------

      value = arr[x_color + 1 + y_color * X + layerNumber * X * Y];

      color = transfer_function(value) / 255.f;
      glColor3f(color, color, color);
      glVertex2f(x_color + 1, y_color);
    }

  glEnd();

}
void ::View::progressKey(int key, int x, int y) {
  switch(key) {
    case GLUT_KEY_UP:
      currentLayer = clamp<int>(currentLayer + 1, 0, Z - 1);
      break;
    case GLUT_KEY_DOWN:
      currentLayer = clamp<int>(currentLayer - 1, 0, Z - 1);
      break;
    case GLUT_KEY_LEFT:
      min = clamp<int>(min - 50, -3000, 0);
      max = clamp<int>(max - 500, 0, 16000);
      break;
    case GLUT_KEY_RIGHT:
      min = clamp<int>(min + 100, -3000, 0);
      max = clamp<int>(max + 100, 0, 16000);
      break;
  }
  glutPostRedisplay ();
}

void ::View::render() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  drawQuads(currentLayer);

  glutSwapBuffers();
}

