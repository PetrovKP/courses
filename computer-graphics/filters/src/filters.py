#
# Скрипт описывающий классы обработки
# изображения с помощью различных фильтров
#

from abc import ABCMeta, abstractmethod
from PyQt5.QtGui import QImage, QColor, qRgb
import numpy as np

class Filter():
    __metaclass__ = ABCMeta

    def __init__(self, pix):

        from main import ProgressDialog
        if pix.isNull():
            raise Exception("Нет картинки!")
        else:
            self.pix = QImage(pix)

        self._thread = ProgressDialog()
        self._thread.start()
        self._thread.sendSep.connect(self._thread.updateProgress)

    def _clamp(self, value, min, max):
        if value < min:
            return min
        if value > max:
            return max
        return value

    @abstractmethod
    def _calculateNewPixelColor(self, x, y):
        """Перерасчёт каждого пикселя"""

    def run(self):
        result = self.pix.copy()

        for i in range(result.width ()):
            if not self._thread.running:
                return self.pix
            sepI = int(i/result.width()*100)
            self._thread.sendSep.emit(sepI)

            for j in range(result.height()):
                result.setPixel(i, j, self._calculateNewPixelColor(i, j))

        self.pix = result
        self._thread.sendSep.emit(100)
        return self.pix


class PointsFilter(Filter):
    def __init__(self, pix):
        super().__init__(pix)

    @abstractmethod
    def _calculateNewPixelColor (self, x, y):
        """Перерасчёт каждого пикселя"""


class MatrixFilter(Filter):
    def __init__(self, pix):
        super().__init__(pix)
        self.kernel = np.zeros((3, 3))

    def _calculateNewPixelColor(self, x, y):
        pix = self.pix.copy()
        radiusX, radiusY = len(self.kernel) // 2, len(self.kernel[0]) // 2
        resultRGB = [0., 0., 0.]
        for l in range(-radiusY, radiusY + 1):
            for k in range(-radiusX, radiusX + 1):
                idX = self._clamp(x + k, 0 , pix.width() - 1)
                idY = self._clamp(y + l, 0, pix.height() - 1)

                neighborColor = QColor(pix.pixel(idX, idY)).getRgb()

                for i, color in zip(range(0,3), neighborColor):
                    resultRGB[i] += color * self.kernel[k + radiusX][l + radiusY]

        return qRgb(
            self._clamp(resultRGB[0], 0, 255),
            self._clamp(resultRGB[1], 0, 255),
            self._clamp(resultRGB[2], 0, 255),
        )



class MorphologyFilter(Filter):
    def __init__ (self, pix):
        super ().__init__(pix)
        self.mask = np.array ([[1, 1, 1], [1, 1, 1], [1, 1, 1]])
        self.radiusY, self.radiusX = len (self.mask[0]) // 2, len (self.mask) // 2

    def run(self):
        result = self.pix.copy()

        for i in range(self.radiusX, result.width() - self.radiusX):

            if not self._thread.running:
                return self.pix
            sepI = int (i / result.width() * 100)
            self._thread.sendSep.emit (sepI)

            for j in range(self.radiusY, result.height() - self.radiusY):
                result.setPixel (i, j, self._calculateNewPixelColor (i, j))

        self.pix = result
        self._thread.sendSep.emit (100)
        return self.pix

    @abstractmethod
    def _calculateNewPixelColor (self, x, y):
        """Перерасчёт каждого пикселя"""
