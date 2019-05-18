# Copyright 2018 Kirill Petrov

file(GLOB src "main.cpp")
file(GLOB src_runge "runge-kutta.cpp")
get_filename_component(parent ${src} DIRECTORY)
get_filename_component(target ${parent} NAME)

find_package(OpenMP REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  ${OpenMP_C_FLAGS}")
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}    ${OpenMP_C_FLAGS}")

add_definitions(-DUSE_HEADER)

add_executable(app_${target}.exe ${src})
add_catch(test_${target} test.cpp)
