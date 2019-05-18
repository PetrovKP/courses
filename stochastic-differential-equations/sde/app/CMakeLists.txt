# Copyright 2019 Kirill Petrov
set(target ${APP})

file(GLOB src "main.cpp")

add_executable(${target} ${src})
message(STATUS "${MKL_LIBRARIES}")
target_include_directories(${target} PRIVATE ${MKL_INCLUDE_DIRS})
target_link_libraries(${target} ${MKL_LIBRARIES})
