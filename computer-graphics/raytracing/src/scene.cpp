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
  GLfloat z;
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

  attribute_vpos = glGetAttribLocation(g_program, "vPosition");
  uniform_campos = glGetUniformLocation(g_program, "cameraPosition");
  uniform_aspect = glGetUniformLocation(g_program, "aspect");

}
void Scene::initVBO() {
  vertex vertdata[] = {
      { -1.f, 1.f, 0.f},

      { 1.f, 1.f, 0.f},

      { 1.f, -1.f, 0.f},


      {-1.f,-1.f, 0.f},

  };

  glGenBuffers(1, &vbo_pos);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_pos);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertdata), vertdata, GL_STATIC_DRAW);
}

void Scene::render() {
  glClear(GL_COLOR_BUFFER_BIT);
  // ! Устанавливаем шейдерную программу текущей

  glUseProgram(g_program);
  // ! Включаем массив атрибутов
  /* setup camera */
  g_cameraPosition[0] = 0.0f;
  g_cameraPosition[1] = 0.0f;
  g_cameraPosition[2] = -4.0f;
  glLoadIdentity();
  glTranslatef(-g_cameraPosition[0], -g_cameraPosition[1], -g_cameraPosition[2]);

  glUniform3fv(uniform_campos, -1, g_cameraPosition);
  glUniform1f(uniform_aspect, 1);

  glEnableVertexAttribArray(attribute_vpos);

  // ! Указывая pointer 0 при подключенном буфере, мы указываем, что данные представлены в VBO
  // ! Отключаем VBO
  glBindBuffer(GL_ARRAY_BUFFER, vbo_pos);
  // ! Передаем данные на видеокарту (рисуем)

  glVertexAttribPointer(attribute_vpos, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  /* disable program */

//  glUseProgram(g_program);
  glDrawArrays(GL_QUADS, 0, sizeof (vertex));

  // ! Отключаем массив атрибутов
  glDisableVertexAttribArray(attribute_vpos);

  // ! Отключаем шейдерн  ую программу
  glUseProgram(0);
  glutSwapBuffers();

}



