# Copyright 2019 Kirill Petrov

file(GLOB src "main.cpp")
get_filename_component(parent ${src} DIRECTORY)
get_filename_component(target ${parent} NAME)

set(omp_target "omp_${target}")
set(tbb_target "tbb_${target}")
set(omp_test_target "omp_test_${target}")
set(tbb_test_target "tbb_test_${target}")

find_package(OpenMP REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  ${OpenMP_C_FLAGS}")
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}    ${OpenMP_C_FLAGS}")

find_package(TBB)

add_executable(${omp_target} ${src})
add_catch(${omp_test_target} test.cpp)

if(${TBB_FOUND})
    add_executable(${tbb_target} ${src})
    add_catch(${tbb_test_target} test.cpp)

    target_include_directories(${tbb_target} PRIVATE ${TBB_INCLUDE_DIRS})
    target_include_directories(${tbb_test_target} PRIVATE ${TBB_INCLUDE_DIRS})

    target_compile_definitions(${tbb_target} PRIVATE -DUSE_TBB)
    target_compile_definitions(${tbb_test_target} PRIVATE -DUSE_TBB)

    target_link_libraries(${tbb_target} ${TBB_LIBRARIES})
    target_link_libraries(${tbb_test_target} ${TBB_LIBRARIES})
endif()
