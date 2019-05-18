//
// Created by kirill on 26.10.16.
//

#include <iostream>
#include <fstream>
#include <vector>

#include <stdlib.h>

#include "opengl.h"

struct vertex
{
  GLfloat x;
  GLfloat y;
};

void Scene::setup() {
  Scene::init();
  Scene::initVBO();
}


void Scene::display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  Scene::render();

  glutSwapBuffers();
}

void Scene::render() {
  glClear(GL_COLOR_BUFFER_BIT);
  // ! Устанавливаем шейдерную программу текущей
  glUseProgram(g_program);

  static float yellow[4] = {1.0f, 1.0f, 0.0f, 1.0f};
  // ! Передаем юниформ в шейдер
  glUniform4fv(Unif_color, 1, yellow);

  // ! Включаем массив атрибутов
  glEnableVertexAttribArray(Attrib_vertex);
  // ! Подключаем VBO
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // ! Указывая pointer 0 при подключенном буфере, мы указываем, что данные представлены в VBO
  glVertexAttribPointer(Attrib_vertex, 2, GL_FLOAT, GL_FALSE, 0, 0);
  // ! Отключаем VBO
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  // ! Передаем данные на видеокарту (рисуем)
  glDrawArrays(GL_TRIANGLES, 0, sizeof (vertex));

  // ! Отключаем массив атрибутов
  glDisableVertexAttribArray(Attrib_vertex);

  // ! Отключаем шейдерную программу
  glUseProgram(0);

  glutSwapBuffers();
}
void Scene::init() {
  GLint result;

  /* create program object and attach shaders */
  GLenum err = glewInit();
  if (err != GLEW_OK)
    exit(1); // or handle the error in a nicer way
  if (!GLEW_VERSION_2_1)  // check that the machine supports the 2.1 API.
    exit(1); // or handle the error in a nicer way

  g_program = glCreateProgram();
  fprintf(stderr, "KEK!\n");
  Shader::attachFromFile(g_program, GL_VERTEX_SHADER, "../shaders/vertex.glsl");
  Shader::attachFromFile(g_program, GL_FRAGMENT_SHADER, "../shaders/fragment.glsl");

  /* link the program and make sure that there were no errors */
  glLinkProgram(g_program);
  glGetProgramiv(g_program, GL_LINK_STATUS, &result);
  if(result == GL_FALSE) {
    GLint length;
    char *log;

    /* get the program info log */
    glGetProgramiv(g_program, GL_INFO_LOG_LENGTH, &length);
    log = new char [length];
    glGetProgramInfoLog(g_program, length, &result, log);

    /* print an error message and the info log */
    fprintf(stderr, "init(): Program linking failed: %s\n", log);
    delete[] log;

    /* delete the program */
    glDeleteProgram(g_program);
    g_program = 0;
  }

  const char* attr_name = "coord";
  Attrib_vertex = glGetAttribLocation(g_program, attr_name);

  const char* unif_name = "color";
  Unif_color = glGetUniformLocation(g_program, unif_name);

}

void Scene::initVBO() {
  glGenBuffers(1, &Scene::VBO);
  glBindBuffer(GL_ARRAY_BUFFER, Scene::VBO);
  // ! Вершины нашего треугольника
  vertex triangle[3] = {
      {-0.8f,-0.8f},
      { 0.0f, 0.8f},
      { 0.8f,-0.8f}
  };
  //! Передаем вершины в буфер
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);
}



