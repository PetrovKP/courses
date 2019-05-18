# Copyright 2018 Kirill Petrov

file(GLOB components "*/main.cpp")

foreach(component ${components})
    string(REPLACE "/main.cpp" "" target ${component})
    get_filename_component(target ${target} NAME)
    file(COPY ${target}/input.txt DESTINATION ${CMAKE_BINARY_DIR}/labs/${target})
    get_filename_component(dir ${component} PATH)
    add_subdirectory(${dir})
endforeach()
