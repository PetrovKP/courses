# Copyright 2018 Kirill Petrov

cmake_minimum_required(VERSION 3.7)
project(Search_Tree)

set(CMAKE_MODULE_PATH             "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_EXPORT_COMPILE_COMMANDS  ON)

set(CMAKE_MODULE_PATH ${CMAKE_HOME_DIRECTORY}/cmake)
include(cmake/BuildFlags.cmake)

get_filename_component(DIR_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
set(MODULE "${DIR_NAME}")

find_package(OpenMP)
find_package(AVX)

if(OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
endif()

add_subdirectory(source)
