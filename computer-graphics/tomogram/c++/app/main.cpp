#include <GL/glut.h>
#include <iostream>

#include "view.h"
//Main program

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  View::width = 600;
  View::heigth = 400;
  glutInitWindowSize(View::width, View::heigth);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow("CT Visualizer");

  View::setup();

  glutDisplayFunc(View::display);

  glutIdleFunc(View::render);
  glutSpecialFunc(View::progressKey);

  glutMainLoop();
  return 0;
}