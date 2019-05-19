cmake_minimum_required(VERSION 2.8)
project(image-processing)

if (UNIX)
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS}     -std=c++14")
    set(CMAKE_C_FLAGS    "${CMAKE_C_FLAGS}       -std=c11")
endif(UNIX)

# ПОИСК библиотеки OpenCV
find_package(OpenCV REQUIRED)
if (NOT OpenCV_FOUND)
    message(FATAL_ERROR "OpenCV not found!")
endif()

get_filename_component(DIR_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
set(MODULE "${DIR_NAME}")

add_subdirectory(modules)
