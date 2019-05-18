from PyQt5.QtGui import qRgb, QImage
from filters import MorphologyFilter
import numpy as np


# Морфологическое сужение
class ErosionFilter(MorphologyFilter):
    def __init__(self, pix):
        super().__init__(pix)

    def _calculateNewPixelColor(self, x, y):

        pix = self.pix.copy()

        minPixel = qRgb(255, 255, 255)

        for l in range(-self.radiusX, self.radiusX + 1):
            for k in range(-self.radiusY, self.radiusY + 1):

                pixel = pix.pixel(x+l, y+k)
                if self.mask[l][k] and pixel < minPixel:
                    minPixel = pixel

        return minPixel


# Морфологическое расширение
class DilationFilter(MorphologyFilter):
    def __init__(self, pix):
        super().__init__(pix)

    def _calculateNewPixelColor(self, x, y):

        pix = self.pix.copy()

        maxPixel = qRgb(0, 0, 0)

        for l in range(-self.radiusX, self.radiusX + 1):
            for k in range(-self.radiusY, self.radiusY + 1):

                pixel = pix.pixel(x+l, y+k)
                if self.mask[l][k] and pixel > maxPixel:
                    maxPixel = pixel

        return maxPixel


# Морфологическое открытие
class OpeningFilter():
    def __init__(self, pix):
        if pix.isNull ():
            raise Exception ("Нет картинки!")
        else:
            self.pix = QImage (pix)

    def run(self):
        result = ErosionFilter(self.pix).run()
        result = DilationFilter(result).run()

        self.pix = result
        return self.pix


# Морфологическое закрытие
class ClosingFilter():
    def __init__ (self, pix):
        if pix.isNull ():
            raise Exception ("Нет картинки!")
        else:
            self.pix = QImage (pix)

    def run(self):
        result = DilationFilter(self.pix).run()
        result = ErosionFilter(result).run()

        self.pix = result
        return self.pix



class HitOrMiss():
    def __init__ (self, pix):
        if pix.isNull ():
            raise Exception ("Нет картинки!")
        else:
            self.pix = QImage (pix)

    def run (self):
        result = ErosionFilter (self.pix).run ()
        result = DilationFilter (result).run ()

        self.pix = result
        return self.pix