# Copyright 2019 Kirill Petrov

get_filename_component(DIR_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
set(MODULE  "${DIR_NAME}")
set(APP     "app_${DIR_NAME}")
set(LIB     "lib_${DIR_NAME}")
set(TESTS   "tests_${DIR_NAME}")

set(ROOT "${CMAKE_CURRENT_LIST_DIR}")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(app)
add_subdirectory(lib)
# add_subdirectory(test)
