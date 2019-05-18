cmake_minimum_required(VERSION 2.8)
project(cuda_labs)

set(CMAKE_MODULE_PATH ${CMAKE_HOME_DIRECTORY}/cmake)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")

set(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} -O3")

find_library(M_LIB m)

find_package(CUDA REQUIRED)
find_package(OpenCL REQUIRED)
find_package(OpenMP)

if(OPENMP_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
endif()

set(LIBUTILS "libutils")

add_definitions(-DKERNEL_CL_PATH="${CMAKE_SOURCE_DIR}/gemm/src/gpu/gemm.cl")

add_definitions(-DGPU_CUDA)

include_directories(utils)
#add_subdirectory(hello)
add_subdirectory(utils)
add_subdirectory(axpy)
add_subdirectory(gemm)


