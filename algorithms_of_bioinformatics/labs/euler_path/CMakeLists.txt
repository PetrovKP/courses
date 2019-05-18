# Copyright 2018 Kirill Petrov


file(GLOB src "main.cpp")

get_filename_component(parent ${src} DIRECTORY)
get_filename_component(target ${parent} NAME)

find_package(LEMON REQUIRED)
include_directories(${LEMON_INCLUDE_DIR})

set(app "app_${target}.exe")
add_executable(${app} ${src})
target_link_libraries(${app} ${LEMON_LIBRARIES})

