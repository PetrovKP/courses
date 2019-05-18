set(target ${APP})

file(GLOB hrcs  "${ROOT}/*.h")
file(GLOB srcs  "*.c")

add_executable(${target} ${hrcs} ${srcs})
target_link_libraries(${target} ${LIB})
