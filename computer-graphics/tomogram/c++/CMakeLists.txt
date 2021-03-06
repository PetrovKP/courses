cmake_minimum_required(VERSION 2.8)
project(Tomogram)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_definitions(${CMAKE_CXX_FLAGS})

include_directories("./include")

set(MODULE      "${PROJECT_NAME}")
set(LIBRARY     "lib_${MODULE}")
set(APPLICATION "app_${MODULE}")

add_subdirectory(src)
add_subdirectory(app)

#########################################################
# FIND GLUT
#########################################################

find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
if(NOT GLUT_FOUND)
    message(ERROR "QQQQ GLUT not found!")
endif(NOT GLUT_FOUND)

#########################################################
# FIND OPENGL
#########################################################
find_package(OpenGL REQUIRED)

include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR "QQQQ OPENGL not found!")
endif(NOT OPENGL_FOUND)

#########################################################

