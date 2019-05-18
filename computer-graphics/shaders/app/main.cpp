#include <GL/glew.h>
#include <GL/glut.h>

#include <iostream>

#include "opengl.h"

static const GLfloat g_vertex_buffer_data[] = {
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    0.0f,  1.0f, 0.0f,
};

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  Scene::width = 600;
  Scene::heigth = 400;
  glutInitWindowSize(Scene::width, Scene::heigth);
  glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE);
  glutCreateWindow("shaders_test");

  Scene::setup();

  glutDisplayFunc(Scene::render);
  glutMainLoop();
  return 0;
}