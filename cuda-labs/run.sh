#!/usr/bin/env bash

TARGET=$1

mkdir _build
cd _build
cmake ..
make
cd $TARGET
echo '_app'$TARGET

#./'$TARGET'_app
