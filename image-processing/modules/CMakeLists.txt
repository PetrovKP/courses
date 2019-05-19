include_directories(include)

set(MODULE      "${DIR_NAME}")
set(APP         "app_${MODULE}")
set(LIB         "lib_${MODULE}")

add_subdirectory(src)
add_subdirectory(app)