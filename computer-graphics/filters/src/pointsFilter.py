from PyQt5.QtGui import QColor, qRgb
from filters import PointsFilter


# Инверсия
class InverseFilter(PointsFilter):
    def __init__(self, pix):
        super().__init__(pix)

    def _calculateNewPixelColor(self, x, y):
        color = QColor(self.pix.pixel(x, y))
        newRhb = (255 - i for i in color.getRgb())
        return qRgb(next(newRhb), next(newRhb), next(newRhb))


# Сепия
class SepiaFilter(PointsFilter):
    def __init__(self, pix):
        super().__init__(pix)

    def _calculateNewPixelColor(self, x,y):
        color = QColor(self.pix.pixel(x,y))
        inputRed, inputGreen, inputBlue = color.getRgb()[:3]

        intensity = 0.36*inputRed + 0.53*inputGreen + 0.11*inputBlue

        COEFF = 35

        outputRed = self._clamp(intensity + 2*COEFF, 0, 255)
        outputGreen = self._clamp(intensity + 0.5*COEFF, 0, 255)
        outputBlue = self._clamp(intensity - COEFF, 0, 255)

        return qRgb(outputRed, outputGreen, outputBlue)


# Серый
class GrayScaleFilter(PointsFilter):
    def __init__(self, pix):
        super().__init__(pix)

    def _calculateNewPixelColor(self, x, y):
        color = QColor(self.pix.pixel (x, y))
        inputRed, inputGreen, inputBlue = color.getRgb()[:3]

        intensity = 0.36*inputRed + 0.53*inputGreen + 0.11*inputBlue

        return qRgb(intensity, intensity, intensity)


# Увеличение яркости
class BrightnessFilter(PointsFilter):
    def __init__(self, pix):
        super().__init__(pix)

    def _calculateNewPixelColor(self, x,y):
        color = QColor(self.pix.pixel(x, y))
        inputRed, inputGreen, inputBlue = color.getRgb()[:3]

        COEFF = 50

        outputRed = self._clamp(inputRed + COEFF, 0, 255)
        outputGreen = self._clamp(inputGreen + COEFF, 0, 255)
        outputBlue = self._clamp(inputBlue + COEFF, 0, 255)

        return qRgb(outputRed, outputGreen, outputBlue)


# Эффект стекла
class GlassFilter(PointsFilter):
    def __init__(self, pix):
        super().__init__(pix)

    def _calculateNewPixelColor(self, x, y):
        from random import random

        newX = self._clamp(x + (random() - 0.5)*10, 0, self.pix.width() - 1)
        newY = self._clamp(y + (random() - 0.5)*10, 0, self.pix.height() - 1)
        pixel = self.pix.pixel(newX, newY)

        return pixel


# Эффект волн
class WavesFilter(PointsFilter):
    def __init__(self, pix):
        super().__init__(pix)

    def _calculateNewPixelColor(self, x, y):
        from math import sin, pi

        newX = self._clamp(x + 20*sin(2*y/60), 0, self.pix.width() - 1)
        newY = y
        pixel = self.pix.pixel(newX, newY)

        return pixel


class BinFilter(PointsFilter):
    def __init__ (self, pix):
        super ().__init__ (pix)

    def _calculateNewPixelColor (self, x, y):
        color = QColor (self.pix.pixel (x, y))

        result = qRgb(0, 0, 0) if all(c < 127 for c in color.getRgb ()[:3]) else qRgb(255, 255, 255)

        return result