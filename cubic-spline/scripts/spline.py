import numpy as np
import matplotlib.pyplot as plt
import spline as spl
import math as mt

def f1(x):
	return x**3 - 3*x**2 + 4

def f2(x):
	return mt.sin(x)/x

def f3(x):
	return mt.cos(2*x)*x

f = f3
a, b = -100, 100


xS = list(np.linspace(a, b, 10))
yS = list(map(f, xS))

s = spl.spline(xS, yS)

xR = list(np.linspace(a, b, 100))
yR = list(map(s.interpolate, xR))
yE = list(map(f, xR))


plt.plot(xR, yE, 'r--', label ='expected function')
plt.plot(xR, yR, label ='result spline')
plt.plot(xS, yS, 'ro', markerfacecolor='black', markeredgecolor='black', markersize=4)
plt.title('Cubic Spline')
plt.legend()
plt.show()
