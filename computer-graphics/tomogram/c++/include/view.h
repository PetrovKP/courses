//
// Created by kirill on 26.10.16.
//

#ifndef TOMOGRAM_VIEW_H_H
#define TOMOGRAM_VIEW_H_H

#include "bin.h"

namespace View {
void setup();

void display();
void render();
void progressKey(int key, int x, int y);

void drawQuads(int layerNumber);

static int width, heigth;
static int currentLayer;
static int min, max;

static int X, Y, Z;
static array arr;
};



#endif //TOMOGRAM_VIEW_H_H
