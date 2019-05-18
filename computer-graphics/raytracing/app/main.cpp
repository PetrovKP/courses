#include "opengl.h"

#include <iostream>

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  Scene::width = 600;
  Scene::heigth = 600;
  glutInitWindowSize(Scene::width, Scene::heigth);
  glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE);
  glutCreateWindow("shaders_test");

  Scene::setup();

  glutDisplayFunc(Scene::render);
  glutMainLoop();
  return 0;
}
