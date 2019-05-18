from PyQt5.QtGui import QColor, qRgb
from filters import MatrixFilter
import numpy as np


# Размытие
class BlurFilter(MatrixFilter):
    def __init__(self, pix):
        super().__init__(pix)
        self.kernel = np.array([[1/9 for i in range(3)] for j in range(3)])


# Размытие по гауссу
class GaussianFilter(MatrixFilter):
    def __init__(self, pix):
        super().__init__(pix)
        self._createGaussianKernel(2,1)

    def _createGaussianKernel(self, radius, sigma):
        from math import exp, pi
        size = 2 * radius + 1
        kernel = np.array([[0. for i in range(size)] for j in range(size)])

        for i in range(-radius, radius + 1):
            for j in range(-radius, radius + 1):
                kernel[i + radius][j + radius] = 1/(2 * pi * sigma**2) * exp(-(i ** 2 + j ** 2) / 2 * sigma**2)

        self.kernel = kernel


# Резкость
class HarshnessFilter(MatrixFilter):
    def __init__ (self, pix):
        super ().__init__(pix)
        self.kernel = np.array([[0, -1, 0],
                                [-1, 5, -1],
                                [0, -1, 0]])


# Выделение границ
class SobelFilter(MatrixFilter):
    def __init__(self, pix):
        super ().__init__(pix)
        self.Gx = np.array([[-1, 0, 1],
                            [-2, 0, 2],
                            [-1, 0, 1]])

        self.Gy = np.array([[-1, -2, -1],
                            [0, 0, 0],
                            [1, 2, 1]])

    def _calculateNewPixelColor(self, x, y):

        radiusX, radiusY = 1, 1

        resultXRGB = [0., 0., 0.]
        resultYRGB = [0., 0., 0.]

        for l in range(-radiusY, radiusY + 1):
            for k in range(-radiusX, radiusX + 1):
                idX = self._clamp(x + k, 0, self.pix.width() - 1)
                idY = self._clamp(y + l, 0, self.pix.height() - 1)

                neighborColor = QColor(self.pix.pixel(idX, idY)).getRgb()

                for i, color in zip(range(0,3), neighborColor):
                    resultXRGB[i] += color * self.Gx[k + radiusX][l + radiusY]
                    resultYRGB[i] += color * self.Gy[k + radiusX][l + radiusY]

        from math import sqrt
        resultRGB = (sqrt(self._clamp(x,0,255)**2 + self._clamp(y,0,255)**2)
                    for x, y in zip(resultXRGB, resultYRGB))

        return qRgb(
            self._clamp(next(resultRGB), 0, 255),
            self._clamp(next(resultRGB), 0, 255),
            self._clamp(next(resultRGB), 0, 255),
        )


# Выделение границ (Щарра)
class FeldmanFilter(SobelFilter):
    def __init__(self, pix):
        super().__init__(pix)
        self.Gx = np.array([[3, 0, -3],
                            [10, 0, -10],
                            [3, 0, -3]])

        self.Gy = np.array([[3, 10, 3],
                            [0, 0, 0],
                            [-3, -10, -3]])



# Размытие с сдвигом
class MotionBlurFilter(MatrixFilter):
    def __init__(self, pix):
        super().__init__(pix)
        N = 9
        self.kernel = np.array([[1/N if i == j else 0. for i in range(N)] for j in range(N)])


# Разница собеля и щарра
class DiffSobelAndFeldmanFilter(MatrixFilter):
    def __init__(self, pix):
        super ().__init__(pix)
        self.SobelX = np.array([[-1, 0, 1], [-2, 0, 2], [-1, 0, 1]])
        self.SobelY = np.array([[-1, -2, -1], [0, 0, 0], [1, 2, 1]])

        self.FeldmanX = np.array([[3, 0, -3], [10, 0, -10], [3, 0, -3]])
        self.FeldmanY = np.array([[3, 10, 3], [0, 0, 0], [-3, -10, -3]])

    def _calculateNewPixelColor(self, x, y):

        radiusX, radiusY = 1, 1

        resultSobelXRGB = [0., 0., 0.]
        resultSobelYRGB = [0., 0., 0.]

        resultFeldmanXRGB = [0., 0., 0.]
        resultFeldmanYRGB = [0., 0., 0.]

        for l in range(-radiusY, radiusY + 1):
            for k in range(-radiusX, radiusX + 1):
                idX = self._clamp(x + k, 0, self.pix.width () - 1)
                idY = self._clamp(y + l, 0, self.pix.height () - 1)

                neighborColor = QColor(self.pix.pixel (idX, idY)).getRgb ()

                for i, color in zip(range(0, 3), neighborColor):
                    resultSobelXRGB[i] += color * self.SobelX[k + radiusX][l + radiusY]
                    resultSobelYRGB[i] += color * self.SobelY[k + radiusX][l + radiusY]

                    resultFeldmanXRGB[i] += color * self.FeldmanX[k + radiusX][l + radiusY]
                    resultFeldmanYRGB[i] += color * self.FeldmanY[k + radiusX][l + radiusY]

        from math import sqrt
        resultSobelRGB = (sqrt(self._clamp(x, 0, 255) ** 2 + self._clamp(y, 0, 255) ** 2)
                            for x, y in zip(resultSobelXRGB, resultSobelYRGB))

        resultFeldmanRGB = (sqrt(self._clamp (x, 0, 255) ** 2 + self._clamp(y, 0, 255) ** 2)
                            for x, y in zip(resultFeldmanXRGB, resultFeldmanYRGB))

        resultRGB = (self._clamp(x - y, 0, 255) for x, y in zip(resultSobelRGB, resultFeldmanRGB))

        return qRgb (self._clamp(next(resultRGB), 0, 255), self._clamp(next(resultRGB), 0, 255),
            self._clamp(next(resultRGB), 0, 255))



# Разница щарра и собеля
class DiffFeldmanAndSobelFilter(MatrixFilter):
    def __init__(self, pix):
        super ().__init__(pix)
        self.SobelX = np.array([[-1, 0, 1], [-2, 0, 2], [-1, 0, 1]])
        self.SobelY = np.array([[-1, -2, -1], [0, 0, 0], [1, 2, 1]])

        self.FeldmanX = np.array([[3, 0, -3], [10, 0, -10], [3, 0, -3]])
        self.FeldmanY = np.array([[3, 10, 3], [0, 0, 0], [-3, -10, -3]])

    def _calculateNewPixelColor(self, x, y):

        radiusX, radiusY = 1, 1

        resultSobelXRGB = [0., 0., 0.]
        resultSobelYRGB = [0., 0., 0.]

        resultFeldmanXRGB = [0., 0., 0.]
        resultFeldmanYRGB = [0., 0., 0.]

        for l in range(-radiusY, radiusY + 1):
            for k in range(-radiusX, radiusX + 1):
                idX = self._clamp(x + k, 0, self.pix.width () - 1)
                idY = self._clamp(y + l, 0, self.pix.height () - 1)

                neighborColor = QColor(self.pix.pixel (idX, idY)).getRgb ()

                for i, color in zip(range(0, 3), neighborColor):
                    resultSobelXRGB[i] += color * self.SobelX[k + radiusX][l + radiusY]
                    resultSobelYRGB[i] += color * self.SobelY[k + radiusX][l + radiusY]

                    resultFeldmanXRGB[i] += color * self.FeldmanX[k + radiusX][l + radiusY]
                    resultFeldmanYRGB[i] += color * self.FeldmanY[k + radiusX][l + radiusY]

        from math import sqrt
        resultSobelRGB = (sqrt(self._clamp(x, 0, 255) ** 2 + self._clamp(y, 0, 255) ** 2)
                            for x, y in zip(resultSobelXRGB, resultSobelYRGB))

        resultFeldmanRGB = (sqrt(self._clamp (x, 0, 255) ** 2 + self._clamp(y, 0, 255) ** 2)
                            for x, y in zip(resultFeldmanXRGB, resultFeldmanYRGB))

        resultRGB = (self._clamp(y - x, 0, 255) for x, y in zip(resultSobelRGB, resultFeldmanRGB))

        return qRgb (self._clamp(next(resultRGB), 0, 255), self._clamp(next(resultRGB), 0, 255),
            self._clamp(next(resultRGB), 0, 255))



class ThinningFilter2(MatrixFilter):
    def __init__(self, pix):
        super ().__init__(pix)
        self.Gx = np.array([[-1, 0, 1],
                            [-2, 0, 2],
                            [-1, 0, 1]])

        self.Gy = np.array([[-1, -2, -1],
                            [0, 0, 0],
                            [1, 2, 1]])

    def _calculateNewPixelColor(self, x, y):

        radiusX, radiusY = 1, 1

        resultXRGB = [0., 0., 0.]
        resultYRGB = [0., 0., 0.]

        for l in range(-radiusY, radiusY + 1):
            for k in range(-radiusX, radiusX + 1):
                idX = self._clamp(x + k, 0, self.pix.width() - 1)
                idY = self._clamp(y + l, 0, self.pix.height() - 1)

                neighborColor = QColor(self.pix.pixel(idX, idY)).getRgb()

                for i, color in zip(range(0,3), neighborColor):
                    resultXRGB[i] += color * self.Gx[k + radiusX][l + radiusY]
                    resultYRGB[i] += color * self.Gy[k + radiusX][l + radiusY]

        from math import sqrt
        resultRGB = (sqrt(self._clamp(x,0,255)**2 + self._clamp(y,0,255)**2)
                    for x, y in zip(resultXRGB, resultYRGB))

        result = qRgb(0, 0, 0) if sum(resultRGB)/3 < 127 else qRgb(255, 255, 255)

        return result
