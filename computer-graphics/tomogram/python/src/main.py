from OpenGL.GL import *
from OpenGL.GLU import *
from OpenGL.GLUT import *

import numpy as np
from math import cos, sin, pi, degrees, radians
import bin

def transfer_function(value):
    _min = 0
    _max = 2000
    intensity = 255*(value - _min)/(_max - _min)
    newValue = np.clip(intensity, 0, 255)
    return newValue


class View:

    def __init__(self, width, heigth):
        self.width, self.heigth = width, heigth
        self.currentLayer = 0

        glutInit(sys.argv)
        glutInitWindowSize(width, heigth)
        glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH)
        glutCreateWindow('CT Visualizer')

        bin.readBin("./../data/testdata.bin")

        print(bin.X, bin.Y)
        self.setupView()

        # регистрация обратных вызовов
        glutDisplayFunc(self.display)
        glutSpecialFunc(self.progressKey)

        # Основной цикл GLUT
        glutMainLoop ()

    def setupView(self):

        glShadeModel(GL_SMOOTH)
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        glOrtho(0, bin.X, 0, bin.Y, -1, 1)
        glViewport(0, 0, self.width, self.heigth)


    def display(self):
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        self.update()

        # САМАЯ ПРЕКРАСНАЯ ВЕЩЬ!!!
        glutSwapBuffers()


    def progressKey(self, key, x, y):
        if key == GLUT_KEY_UP:
            self.currentLayer = np.clip(self.currentLayer + 1, 0, bin.Z - 1)
        elif key == GLUT_KEY_DOWN:
            self.currentLayer = np.clip(self.currentLayer - 1, 0, bin.Z - 1)
        print (self.currentLayer)
        glutPostRedisplay ()



    def drawQuads(self, layerNumber):
        # glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        glBegin(GL_QUADS)
        for x_color in range(bin.X - 1):
            for y_color in range(bin.Y - 1):
                # 1 вершина
                value = bin.arr[x_color + y_color* bin.X +
                                layerNumber * bin.X * bin.Y]

                color = transfer_function(value)/255.0
                glColor3f(color, color, color)
                glVertex2f(x_color, y_color)

                # 2 вершина
                value = bin.arr[x_color + (y_color + 1)*bin.X +
                                layerNumber*bin.X*bin.Y]

                color = transfer_function(value)/255.0
                glColor3f(color, color, color)
                glVertex2f(x_color, y_color + 1)

                # 3 вершина
                value = bin.arr[x_color + 1 + (y_color + 1)*bin.X +
                                layerNumber*bin.X*bin.Y]

                color = transfer_function(value)/255.0
                glColor3f(color, color, color)
                glVertex2f(x_color + 1, y_color + 1)

                # 4 вершина
                value = bin.arr[x_color + 1 + y_color*bin.X +
                                layerNumber*bin.X*bin.Y]

                color = transfer_function(value)/255.0
                glColor3f(color, color, color)
                glVertex2f(x_color+1, y_color)
        glEnd()

    def update (self):
        # работы будет теперь с просмотром
        # glMatrixMode(GL_MODELVIEW)

        # считывает текущую матрицу
        # glLoadIdentity()
        # Устанавливает точку наблюдения, камеру
        # x = self.radius*cos(radians(self.latitude))*cos(radians(self.longitude))
        # y = self.radius*cos(radians(self.latitude))*sin(radians(self.longitude))
        # z = self.radius*sin(radians(self.latitude))

        # gluLookAt(x, y, z,  # откуда (x,y,z)
            # 0, 0, 0,  # куда (x,y,z)
            # 0, 0, 1)  # up-vector

        self.render()

    def render(self):
        self.drawQuads(self.currentLayer)

    # установка освещения
    def setupLightning(self):
        pass

if __name__ == "__main__":
    GL = View(600,400)