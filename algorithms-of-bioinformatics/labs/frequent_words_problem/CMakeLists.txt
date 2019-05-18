# Copyright 2018 Kirill Petrov

file(GLOB src "main.cpp")
get_filename_component(parent ${src} DIRECTORY)
get_filename_component(target ${parent} NAME)

add_executable(app_${target}.exe ${src})
