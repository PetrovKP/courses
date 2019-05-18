from OpenGL.GL import *
from OpenGL.GLU import *
from OpenGL.GLUT import *

import numpy as np
from math import cos, sin, pi, degrees, radians


class GLgraphics():
    offset = 1
    offsetSpeed = -0.01
    startPosition = (47.98, 60.41)
    def __init__(self, width, heigth):
        self.latitude, self.longitude = self.startPosition
        self.radius = 5.385
        self.xrot = self.yrot = self.zrot = 0.
        self.fi = 0.

        self.texturesIDs = []

        self.width, self.heigth = width, heigth

        glutInit (sys.argv)
        glutInitWindowSize (width, heigth)
        glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH)
        glutCreateWindow ('OpenGL')

        self.setup()

        # регистрация обратных вызовов
        glutDisplayFunc (self.display)
        # Обработчик перемещений мыши с нажатой клавишей
        glutMotionFunc (self.mouseMove)
        #
        glutMouseFunc(self.mouseButton)


        # Основной цикл GLUT
        glutMainLoop ()

    def setup(self):
        glClearColor(.5, .5, .5, 1)
        glShadeModel(GL_SMOOTH)
        glEnable(GL_DEPTH_TEST)

        for i in range(1, 7):
            self.texturesIDs.append(self.loadTexture("./../textures/n" + str(i)+ ".jpg"))

        #  Следующие команды относятся к проекту
        glMatrixMode(GL_PROJECTION)

        # Настройка перспективы просмотра
        gluPerspective(
            45,                             # охват в градусах
            self.width/self.heigth,         # угол поворота по оси Y
            1,                              #
            64)                             #

        self.setupLightning()

    def display(self):
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        self.update()

        # САМАЯ ПРЕКРАСНАЯ ВЕЩЬ!!!
        glutSwapBuffers()

    def draw_point(self):
        glPointSize(8)
        glBegin(GL_POINTS)
        glColor3f(1., 0., 1.);
        glVertex3f(0., 0., 0.)
        glEnd()

    def draw_textured_quad(self):
        glEnable(GL_TEXTURE_2D, self.texturesIDs[0])

        glBegin(GL_QUADS)
        glColor3f(0, 0, 1)
        glTexCoord2d(0, 0)
        glVertex3f(-1, -1, -1)
        glColor3f(1, 0, 0)
        glTexCoord2d(0, 1)
        glVertex3f(-1, 1, -1)
        glColor3f(1, 1, 1)
        glTexCoord2d(1, 1)
        glVertex3f(1, 1, -1)
        glColor3f(0, 1, 0)
        glTexCoord2d(1, 0)
        glVertex3f(1, -1, -1)
        glEnd()

        glDisable(GL_TEXTURE_2D)

    def draw_line(self):
        glLineWidth(3)
        glBegin(GL_LINES)
        glColor3f(1., 0., 0.);
        glVertex3f(-1., -1., -1.)
        glColor3f(0., 1., 0.);
        glVertex3f(1., 1., 1.)
        glEnd()

    def draw_triangle(self):
        glBegin(GL_TRIANGLES)
        glColor3f(1., 0., 0.); glVertex3f(0., 1., -1.)
        glColor3f(0., 1., 0.); glVertex3f(-1., 0., -1.)
        glColor3f(0., 0., 1.); glVertex3f(1., 0., -1.)
        glEnd()

    def draw_triangle_strip(self):
        glBegin(GL_TRIANGLE_STRIP)
        glColor3f(1., 0., 0.); glVertex3f(0., 0., -1.)
        glColor3f(0., 1., 0.); glVertex3f(0., 1., -1.)
        glColor3f(0., 0., 1.); glVertex3f(1., 0., -1.)
        glColor3f(1., 1., 1.); glVertex3f(1.5, 1., -1.)
        glEnd()

    def draw_triangle_fan(self):
        glBegin(GL_TRIANGLE_FAN)
        glColor3f(1., 0., 0.); glVertex3f(1., 1.5, -1.)
        glColor3f(0., 0., 1.); glVertex3f(0., 1., -1.)
        glColor3f(0., 1., 0.); glVertex3f(1., 0., -1.)
        glColor3f(1., 1., 1.); glVertex3f(1.5, 1., -1.)
        glEnd()

    def draw_quad (self):
        glColor3f(1., 1., 0.1)
        glBegin(GL_QUADS)
        glColor3f(0., 1., 1.); glVertex3f(-1., -1., -1.)
        glColor3f(0., 1., 1.); glVertex3f(-1., 1., -1.)
        glColor3f(0., 1., 1.); glVertex3f(1., 1., -1.)
        glColor3f(0., 1., 1.); glVertex3f(1., -1., -1.)
        glEnd()

    def draw_sphere(self, r, nx, ny):
        glColor3f(.3,.4, .7)
        for iy in range(ny):
            glBegin(GL_QUAD_STRIP)
            for ix in range(nx + 1):
                x = r*sin(iy*pi/ny)*cos(2*ix*pi/nx)
                y = r*sin(iy*pi/ny)*sin(2*ix*pi/nx)
                z = r*cos(iy*pi/ny)
                glNormal3f(x,y,z)
                glVertex3f(x,y,z)

                x = r*sin((iy+1)*pi/ny)*cos(2*ix*pi/nx)
                y = r*sin((iy+1)*pi/ny)*sin(2*ix*pi/nx)
                z = r*cos((iy+1)*pi/ny)
                glNormal3f(x, y, z)
                glVertex3f(x, y, z)
            glEnd()

    def draw_cube(self):
        glEnableClientState(GL_TEXTURE_COORD_ARRAY)

        glBindTexture(GL_TEXTURE_2D, self.texturesIDs[0])
        glBegin(GL_QUADS); glNormal3f(0., 0., 1.)
        glTexCoord2f(0., 0.); glVertex3f(-1., -1., 1.)
        glTexCoord2f(1., 0.); glVertex3f(1., -1., 1.)
        glTexCoord2f(1., 1.); glVertex3f(1., 1., 1.)
        glTexCoord2f(0., 1.); glVertex3f(-1., 1., 1.)
        glEnd()

        glEnable(GL_TEXTURE_2D, self.texturesIDs[1])
        glBindTexture(GL_TEXTURE_2D, self.texturesIDs[1])

        glBegin(GL_QUADS); glNormal3f(0., 0., -1.)
        glTexCoord2f(1., 0.); glVertex3f(-1., -1., -1.)
        glTexCoord2f(1., 1.); glVertex3f(-1., 1., -1.)
        glTexCoord2f(0., 1.); glVertex3f(1., 1., -1.)
        glTexCoord2f(0., 0.); glVertex3f(1., -1., -1.)

        glEnd()

        glEnable(GL_TEXTURE_2D, self.texturesIDs[5])
        glBindTexture(GL_TEXTURE_2D, self.texturesIDs[5])

        glBegin(GL_QUADS); glNormal3f(0., 1., 0.)
        glTexCoord2f(1., 1.); glVertex3f(-1., 1., -1.)
        glTexCoord2f(1., 0.); glVertex3f(-1., 1., 1.)
        glTexCoord2f(0., 0.); glVertex3f(1., 1., 1.)
        glTexCoord2f(0., 1.); glVertex3f(1., 1., -1.)

        glEnd()

        glEnable(GL_TEXTURE_2D, self.texturesIDs[3])
        glBindTexture(GL_TEXTURE_2D, self.texturesIDs[3])

        glBegin(GL_QUADS); glNormal3f(0., -1., 0.)
        glTexCoord2f(0., 1.); glVertex3f(-1., -1., -1.)
        glTexCoord2f(0., 0.); glVertex3f(-1., -1., 1.)
        glTexCoord2f(1., 0.); glVertex3f(1., -1., 1.)
        glTexCoord2f(1., 1.); glVertex3f(1., -1., -1.)

        glEnd()

        glEnable(GL_TEXTURE_2D, self.texturesIDs[4])
        glBindTexture(GL_TEXTURE_2D, self.texturesIDs[4])

        glBegin(GL_QUADS); glNormal3f(1., 0., 0.)
        glTexCoord2f(0., 1.); glVertex3f(1., -1., -1.)
        glTexCoord2f(1., 1.); glVertex3f(1., 1., -1.)
        glTexCoord2f(1., 0.); glVertex3f(1., 1., 1.)
        glTexCoord2f(0., 0.); glVertex3f(1., -1., 1.)

        glEnd()

        glEnable(GL_TEXTURE_2D, self.texturesIDs[2])
        glBindTexture(GL_TEXTURE_2D, self.texturesIDs[2])

        glBegin(GL_QUADS); glNormal3f(-1., 0., 0.)
        glTexCoord2f(1., 1.); glVertex3f(-1., -1., -1.)
        glTexCoord2f(1., 0.); glVertex3f(-1., -1., 1.)
        glTexCoord2f(0., 0.); glVertex3f(-1., 1., 1.)
        glTexCoord2f(0., 1.); glVertex3f(-1., 1., -1.)

        glEnd()

    def mouseMove(self, x, y):
        widthCoef = (x - self.width*0.5)/self.width
        heigthCoef = (-y + self.heigth*0.5)/self.heigth

        self.latitude = heigthCoef * 180
        self.longitude = widthCoef * 360

        # помечает активное окно,
        # которое должно быть перерисовано,
        # для вызова основного цикла
        glutPostRedisplay ()

    # ПРОДУМАТЬ!
    def mouseButton(self, button, state, x, y):
        if button == GLUT_RIGHT_BUTTON:
            self.latitude, self.longitude = self.startPosition


    def update (self):
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

        # работы будет теперь с просмотром
        glMatrixMode(GL_MODELVIEW)

        # считывает текущую матрицу
        glLoadIdentity()
        # Устанавливает точку наблюдения, камеру
        x = self.radius*cos(radians(self.latitude))*cos(radians(self.longitude))
        y = self.radius*cos(radians(self.latitude))*sin(radians(self.longitude))
        z = self.radius*sin(radians(self.latitude))

        gluLookAt(x, y, z,  # откуда (x,y,z)
            0, 0, 0,  # куда (x,y,z)
            0, 0, 1)  # up-vector

        self.render()

    def render(self):
        self.xPos = 1*cos(radians(self.fi))
        self.yPos = 1*sin(radians(self.fi))

        glPushMatrix()
        glRotatef(self.xrot, 1., 0., 0.)
        glRotatef(self.yrot, 0., 1., 0.)
        glRotatef(self.zrot, 0., 0., 1.)

        glTranslate(self.xPos, self.yPos, 0.)
        glScale(1., 1., 1.)

        # self.draw_point()
        # self.draw_triangle()
        # self.draw_triangle_strip()
        # self.draw_triangle_fan()
        # self.draw_quad()
        # self.draw_line()
        self.draw_cube()
        # self.draw_textured_quad()
        # self.draw_sphere(1, 40, 40)
        # self.xrot = self.xrot + 0.5
        # self.yrot = self.yrot + 0.5
        self.zrot = self.zrot + 0.5
        self.fi += 0.5
        glPopMatrix()

        glutPostRedisplay()

    # загрузка текстуры
    def loadTexture(self, filePath):
        from PIL import Image

        img = Image.open(filePath) # .jpg, .bmp
        img_data = img.tobytes()

        ix, iy = img.size[0], img.size[1]

        ID = glGenTextures(1)
        glBindTexture(GL_TEXTURE_2D, ID)
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1)

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ix, iy,
                     0, GL_RGB, GL_UNSIGNED_BYTE, img_data)
        glGenerateMipmap(GL_TEXTURE_2D)
        return ID

    # установка освещения
    def setupLightning(self):
        glEnable(GL_LIGHTING)
        glEnable(GL_LIGHT0)
        # glEnable(GL_LIGHT1)
        glEnable(GL_COLOR_MATERIAL)

        glLight(GL_LIGHT0, GL_POSITION, np.array([1., 1., 4., 0.], 'f'))
        glLight(GL_LIGHT0, GL_AMBIENT, np.array([.2, .2, .2, 1.], 'f'))
        glLight(GL_LIGHT0, GL_DIFFUSE, np.array([.6, .6, .6, 1.], 'f'))

        # glLight(GL_LIGHT1, GL_POSITION, np.array([1., 1., -4., 0.], 'f'))
        # glLight(GL_LIGHT1, GL_AMBIENT, np.array([.2, .2, .2, 1.], 'f'))
        # glLight(GL_LIGHT1, GL_DIFFUSE, np.array([.9, .8, .6, 1.], 'f'))

        glMaterial(GL_FRONT, GL_SPECULAR, np.array([1., 1., 1., 1.], 'f'))
        glMaterial(GL_FRONT, GL_SHININESS, np.array([100.], 'f'))

if __name__ == "__main__":
    GL = GLgraphics(600,400)