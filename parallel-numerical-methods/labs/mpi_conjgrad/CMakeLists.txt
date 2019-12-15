# Copyright 2019 Kirill Petrov

file(GLOB src "main.cpp")
get_filename_component(parent ${src} DIRECTORY)
get_filename_component(target ${parent} NAME)

set(mpi_target "${target}")
set(test_target "test_${target}")

find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})

add_executable(${mpi_target} ${src})
target_link_libraries(${mpi_target} ${MPI_C_LIBRARIES})

add_catch(${test_target} test.cpp)
target_link_libraries(${test_target} ${MPI_C_LIBRARIES})
