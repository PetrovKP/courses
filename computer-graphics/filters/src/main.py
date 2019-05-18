#
# Основной скрипт программы.
# Запускает конфигуратор окна, подключает слоты и отображает окно.
#


from form import Ui_MainWindow
from PyQt5.QtCore import QThread, QSize, pyqtSignal, Qt
from PyQt5.QtWidgets import QFileDialog, QProgressDialog, QMessageBox, QApplication, QMainWindow
from PyQt5.QtGui import QPixmap

from matrixFilter import *
from pointsFilter import *
from morphology import *

# THinning попробывать )
# morphology

class ProgressDialog(QThread):
    MAXPROGRESS = 100
    sendSep = pyqtSignal(int)

    def __init__(self):
        super().__init__()
        self.progress = QProgressDialog("Идёт выполнение фильтра...", "Отмена", 0, self.MAXPROGRESS, None)
        self.progress.setWindowTitle("Обновление")
        self.progress.setWindowModality(Qt.WindowModal)
        self.progress.setValue(0)
        self.progress.setFixedSize(QSize(300, 150))
        self.progress.show()

        self.running = True

    def cancel(self):
        self.running = False
        self.progress.close()
        self.quit()

    def updateProgress(self, val):
        self.progress.setValue(val)
        if self.progress.wasCanceled() or val == self.MAXPROGRESS:
            self.cancel()

    def run(self):
        self.exec_()


class MainWindow (Ui_MainWindow):
    def __init__(self, form):
        self.pix = QPixmap()
        self.copyPix = QPixmap()

        self.form = form
        self.setupUi(form)
        self.connectSlots()

    # Все слоты (события)
    def connectSlots(self):
        self.openFile.triggered.connect(self.loadImage)
        self.reset.triggered.connect(self.resetImage)
        self.saveFile.triggered.connect(self.saveImage)

        self.inversion.triggered.connect(lambda: self.runFilter(InverseFilter))
        self.sepia.triggered.connect(lambda: self.runFilter(SepiaFilter))
        self.gray.triggered.connect(lambda: self.runFilter(GrayScaleFilter))
        self.bright.triggered.connect(lambda: self.runFilter(BrightnessFilter))
        self.transfer.triggered.connect(lambda: self.runFilter(WavesFilter))
        self.glass.triggered.connect(lambda: self.runFilter(GlassFilter))
        self.bin.triggered.connect(lambda: self.runFilter(BinFilter))

        self.gauss.triggered.connect(lambda: self.runFilter(GaussianFilter))
        self.blue.triggered.connect(lambda: self.runFilter(BlurFilter))
        self.harsness.triggered.connect(lambda: self.runFilter(HarshnessFilter))
        self.sobel.triggered.connect(lambda: self.runFilter(SobelFilter))
        self.feldman.triggered.connect(lambda: self.runFilter(FeldmanFilter))
        self.motionblur.triggered.connect(lambda: self.runFilter(MotionBlurFilter))
        self.diffFS.triggered.connect(lambda: self.runFilter(DiffFeldmanAndSobelFilter))
        self.diffSF.triggered.connect(lambda: self.runFilter(DiffSobelAndFeldmanFilter))

        self.erosion.triggered.connect(lambda: self.runFilter(ErosionFilter))
        self.dilation.triggered.connect(lambda: self.runFilter(DilationFilter))
        self.opening.triggered.connect(lambda: self.runFilter(OpeningFilter))
        self.closing.triggered.connect(lambda: self.runFilter(ClosingFilter))

        self.test.triggered.connect(lambda : self.runFilter(BinFilter))

    # Обновление сцены
    def updateScene(self):
        self.scene.setPixmap(self.pix)

    # Выполнение фильтра
    def runFilter(self, typeFilter):
        # try:
            filter = typeFilter(self.pix)
            self.pix = QPixmap(filter.run())

            self.updateScene()

        # except Exception as error:
        #     QMessageBox.warning(self.form, "Ошибка",
        #                         error.__str__())

    # Загрузка изображение с помощью диалога
    def loadImage(self):
        filename = QFileDialog.getOpenFileName(self.form, 'Открыть', '/home/kirill/labs-for-computer-graphics/img', 'Images (*.png *.xpm *.jpg)')[0]
        if filename != "":
            self.pix = QPixmap(filename)
            self.copyPix = self.pix.copy()

            self.updateScene()

    # Сброс изображения
    def resetImage(self):
        try:
            if self.copyPix.isNull():
                raise Exception("Нечего сбрасывать!")
            if self.copyPix == self.pix:
                raise Exception ("Ничего не изменилось!")
            self.pix = self.copyPix
            self.updateScene()

        except Exception as error:
            QMessageBox.warning (self.form, "Ошибка", error.__str__ ())

    # Сохранене изборажения
    def saveImage(self):
        try:
            if self.pix.isNull():
                raise Exception("Нечего сохранять!")
            fileName = QFileDialog.getSaveFileName(self.form, 'Сохранить', '/home/kirill/labs-for-computer-graphics/img', 'Images (*.png *.xpm *.jpg)')[0]
            self.pix.save(fileName + '.jpg')

        except Exception as error:
            QMessageBox.warning (self.form, "Ошибка", error.__str__ ())

if __name__ == '__main__':
    import sys
    app = QApplication(sys.argv)
    window = QMainWindow()
    ui = MainWindow(window)
    window.show()
    sys.exit(app.exec_())