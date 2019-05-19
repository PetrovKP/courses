cmake_minimum_required(VERSION 2.8)
project(laplace-equation)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

#find_program(ICC_PATH icc)
#find_program(ICPC_PATH icpc)
#
#message(STATUS ${ICC_PATH})
#message(STATUS ${ICPC_PATH})
#
#set(CMAKE_CXX_COMPILER  ${ICPC_PATH})
#set(CMAKE_C_COMPILER ${ICC_PATH})

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_C_FLAGS    "${CMAKE_C_FLAGS}   -std=c11")

#find_package(TBB REQUIRED)
#f(TBB_FOUND)
 #   include_directories(${TBB_INCLUDE_DIRS})
#endif()

find_package(OpenMP REQUIRED)
if(OPENMP_FOUND)
    message(STATUS "OpenMP FOUND")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  ${OpenMP_CXX_FLAGS}")
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}    ${OpenMP_C_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

find_package( MPI REQUIRED )
if(MPI_FOUND)
    include_directories( ${MPI_INCLUDE_PATH} )
endif()

include_directories(include)

get_filename_component(DIR_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
set(MODULE "${DIR_NAME}")

set(MODULE           "${DIR_NAME}")
set(APP              "app_${MODULE}")
set(LIBRARY_SERIAL   "libserial_${MODULE}")
set(LIBRARY_OMP      "libomp_${MODULE}")
set(LIBRARY_TBB      "libtbb_${MODULE}")

add_subdirectory(app)
add_subdirectory(src)
