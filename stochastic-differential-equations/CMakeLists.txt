# Copyright 2018 Kirill Petrov

cmake_minimum_required(VERSION 3.7.2)
project(SDE C CXX)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_EXPORT_COMPILE_COMMANDS  ON)

find_package(OpenMP REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  ${OpenMP_C_FLAGS}")
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}    ${OpenMP_C_FLAGS}")

find_package(Catch REQUIRED)
include(cmake/TestSolution.cmake)
include(cmake/BuildFlags.cmake)
include_directories(util)

# set(MKL_USE_STATIC_LIBS ON)
find_package(MKL REQUIRED)

find_package(PythonLibs 3 REQUIRED)

get_filename_component(DIR_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
set(MODULE "${DIR_NAME}")

add_subdirectory(sde)
