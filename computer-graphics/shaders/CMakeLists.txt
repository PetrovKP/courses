cmake_minimum_required(VERSION 2.8)
project(Sheder)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_definitions(${CMAKE_CXX_FLAGS})

include_directories("./include")

set(MODULE      "${PROJECT_NAME}")
set(LIBRARY     "lib_${MODULE}")
set(APPLICATION "app_${MODULE}")

add_subdirectory(src)
add_subdirectory(app)

#########################################################
#                    FIND GLUT
#########################################################

find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
add_definitions(${GLUT_DEFINITIONS})
if(NOT GLUT_FOUND)
    message(ERROR "GLUT not found!")
endif(NOT GLUT_FOUND)

#########################################################
#                   FIND GLEW
#########################################################

find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
else(GLEW_FOUND)
    message(ERROR "GLEW not found!")
endif(GLEW_FOUND)