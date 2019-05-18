# Copyright 2018 Kirill Petrov

cmake_minimum_required(VERSION 3.7.2)
project(bioinformatics_algorithms CXX)

if(${CMAKE_COMPILER_IS_GNUCXX})
    include(${CMAKE_SOURCE_DIR}/cmake/gnu.cmake)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_HOME_DIRECTORY}/cmake)

get_filename_component(DIR_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
set(MODULE "${DIR_NAME}")

include_directories(include)

add_subdirectory(labs)
