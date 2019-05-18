#!/bin/sh

while [[ $# -ge 1 ]]
do
    key="$1"

    case $key in
        -c|--compiler)
            COMPILER="$2"
            shift
            ;;
        -r|--run-test)
            RUN="$2"
            shift
            ;;
        -h|--help)
            exit 0
            ;;
        *)
            ;;
    esac

    shift
done

root_dir="$( pwd )"

Build () {
    mkdir _build
    cd _build
    echo $root_dir
    cmake -DCMAKE_BUILD_TYPE=DEBUG -DCMAKE_CXX_COMPILER=${COMPILER} $root_dir
    make -j2
}

Clear() {
  if [ -d _build ]
    then
    rm -rf ./_build
  fi
}

RunTest() {
    for filename in `find $root_dir/_build/labs/*/ -type f -name "*.exe"`
    do
        cd `dirname $filename`
        echo "Run `basename $filename`"
        ./`basename $filename`
        echo "Output: "
        cat "output.txt"
        echo
    done
}

BuildLemon() {
    cd lemon-1.3.1
    if [ ! -d _build ]
    then
        mkdir _build
        cd _build
        cmake ..
        make
    fi
    cd $root_dir
}

Clear
BuildLemon
Build
if [ "${RUN}" == "tests" ]; then
    RunTest
fi
