# Copyright 2019 Kirill Petrov

set(target "SDE")

file(GLOB srcs "*.cpp")

execute_process(COMMAND "python3" -c "import numpy; print(numpy.get_include(), end='')"
                OUTPUT_VARIABLE NUMPY_INCLUDE_DIRS
                RESULT_VARIABLE NUMPY_NOTFOUND)

message(STATUS "${NUMPY_INCLUDE_DIRS}")

if(NUMPY_NOTFOUND)
    message(FATAL_ERROR "Numpy not found")
endif()

add_library(${target} SHARED ${srcs})

target_link_libraries(${target} ${MKL_LIBRARIES})

target_include_directories(${target} PRIVATE ${MKL_INCLUDE_DIRS})
target_include_directories(${target} PRIVATE ${PYTHON_INCLUDE_DIRS})
target_include_directories(${target} PRIVATE ${NUMPY_INCLUDE_DIRS})

set_target_properties(${target} PROPERTIES PREFIX "")
