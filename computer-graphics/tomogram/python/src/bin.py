import numpy as np

X, Y, Z = 0, 0, 0
arr = np.array([])

def readBin(path):
    import struct
    with open(path, mode = 'rb') as reader:
        global X, Y, Z, arr
        X, = struct.unpack('i', reader.read(4))
        Y, = struct.unpack('i', reader.read(4))
        Z, = struct.unpack('i', reader.read(4))
        arr = np.fromfile(reader, dtype = np.int16)
