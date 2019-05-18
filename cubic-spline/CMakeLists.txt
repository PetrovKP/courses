cmake_minimum_required(VERSION 2.8)
project(cubic_splain)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_MODULE_PATH ${CMAKE_HOME_DIRECTORY}/cmake)

find_library(M_LIB m)
find_package(MKL REQUIRED)

get_filename_component(DIR_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
set(MODULE  "${DIR_NAME}")
set(APP      "app_${DIR_NAME}")
set(LIB      "lib_${DIR_NAME}")
set(LIBUTILS "lib_utils_${DIR_NAME}")

set(ROOT    "${CMAKE_CURRENT_LIST_DIR}")

include_directories(include)
include_directories(${MKL_INCLUDE_DIRS})

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(utils)
