import numpy as np

def is_pos_def(x):
    print(np.linalg.eigvals(x))
    return np.all(np.linalg.eigvals(x) > 0)

X = np.array([
    [8.57901, -9.64983, 5.60079, -28.4932],
    [-9.64983, 6.60499, -21.1995, 46.339],
    [5.60079, -21.1995, 19.4112, -44.0885],
    [-28.4932, 46.339, -44.0885, 173.896]])

print(is_pos_def(X))