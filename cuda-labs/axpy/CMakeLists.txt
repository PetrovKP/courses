get_filename_component(DIR_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
set(MODULE  "${DIR_NAME}")
set(APP     "app_${DIR_NAME}")
set(LIB     "lib_${DIR_NAME}")
set(LIBGPU  "lib_gpu_${DIR_NAME}")

set(ROOT    "${CMAKE_CURRENT_LIST_DIR}")

include_directories(${ROOT})

add_subdirectory(src)
add_subdirectory(test)
