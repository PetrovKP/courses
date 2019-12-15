# Copyright 2018 Kirill Petrov

file(GLOB components "mpi_conjgrad/main.cpp")

foreach(component ${components})
  string(REPLACE "/main.cpp" "" target ${component})
  get_filename_component(target ${target} NAME)
  get_filename_component(dir ${component} PATH)
  add_subdirectory(${dir})
endforeach()
