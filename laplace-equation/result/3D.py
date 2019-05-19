from mpl_toolkits.mplot3d import Axes3D
from matplotlib import pylab
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import cm
import math
import os

def build_3D_function(ax, title, X, Y, Z, stride):
	# ax.contourf(X, Y, Z)
	ax.plot_surface(X, Y, Z, alpha=0.6, rstride=stride, cstride=stride,
					antialiased=True, shade=False, cmap=cm.coolwarm)

	plt.title(title)

def main():
	pathSetting = os.path.join(os.pardir, "initial", "setting.ini")
	with open(pathSetting) as file:
		setting = {line.split('=')[0] : float(line.split('=')[1]) for line in file}

	N = int(setting['DIM'])

	XSTART = setting['XSTART']
	XEND = setting['XEND']
	YSTART = setting['YSTART']
	YEND = setting['YEND']

	X = np.linspace(XSTART, XEND, N, dtype = float)
	Y = np.linspace (YSTART, YEND, N, dtype = float)
	X, Y = np.meshgrid(X, Y)

	fig = pylab.figure('PLOTS')

	result_Z = np.loadtxt('SerialResult.txt', unpack=True)
	expected_Z = np.sin(math.pi*Y)*np.exp(-math.pi*X)

	result = np.reshape(result_Z, len(result_Z) * len(result_Z[0]))
	expected = np.reshape(expected_Z, len(expected_Z) * len(expected_Z[0]))

	absoluteFault = max([abs(xi - xj) for xi, xj in zip(result, expected)])
	print("абсолютная ошибка ожидаемого и получаемого результата: {:f}".format(absoluteFault))

	stride = N // 20
	print("размер шага равен {:d}".format(stride))

	ax1 = fig.add_subplot(111,  projection = '3d')
	build_3D_function(ax1, 'RESULT', X, Y, result_Z, stride)

	# ax2 = fig.add_subplot(122, projection = '3d')
	# build_3D_function(ax2, 'EXPECTED', X, Y, expected_Z, stride)

	plt.show()

if __name__ == '__main__':
	main()
