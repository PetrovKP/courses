# Copyright 2018 Kirill Petrov

cmake_minimum_required(VERSION 3.7)
project(parallel_algorithms CXX)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Catch REQUIRED)
include(cmake/TestSolution.cmake)
include(cmake/BuildFlags.cmake)
include_directories(util)

find_package(OpenMP REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  ${OpenMP_CXX_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}    ${OpenMP_C_FLAGS}")

get_filename_component(DIR_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
set(MODULE "${DIR_NAME}")

add_subdirectory(labs)
