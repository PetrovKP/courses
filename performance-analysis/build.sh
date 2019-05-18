#!/bin/sh
root_dir="$( pwd )"

Build () {
    mkdir _build
    cd _build
    echo $root_dir
    cmake -DCMAKE_BUILD_TYPE=RELEASE $root_dir
    make -j2
}

Clear() {
  if [ -d _build ]
    then
    rm -rf ./_build
  fi
}

Clear
Build
