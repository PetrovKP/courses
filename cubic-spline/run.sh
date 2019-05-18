#!/usr/bin/env bash

source /opt/intel/bin/compilervars.sh intel64
mkdir _build
cd _build
cmake -DCMAKE_C_COMPILER=icc -DCMAKE_CXX_COMPILER=icpc ..
make
cd ..
cp _build/src/spline.so scripts
