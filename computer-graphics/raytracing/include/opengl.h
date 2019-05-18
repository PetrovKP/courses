//
// Created by kirill on 26.10.16.
//

#ifndef TOMOGRAM_VIEW_H_H
#define TOMOGRAM_VIEW_H_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>

namespace Scene {
void setup();

void display();
void render();

void init();
void initVBO();

static int width, heigth;

static float g_cameraPosition[3];

// ! ID шейдерной программы
static GLuint g_program;
// ! ID атрибута

static GLuint attribute_vpos;
static GLuint uniform_campos;
static GLuint uniform_aspect;

// ! ID юниформ переменной цвета
static GLint  Unif_color;
// ! ID Vertex Buffer Object
static GLuint vbo_pos;

};

namespace Shader {
static char* loadSource(const char *filePath);
static GLuint compileFromFile(GLenum type, const char *filePath);
void attachFromFile(GLuint program, GLenum type, const char *filePath);
}



#endif //TOMOGRAM_VIEW_H_H
