set(target ${APP})

file(GLOB hrcs  "${ROOT}/*.h")
file(GLOB srcs  "*.cpp")

add_executable(${target} ${hrcs} ${srcs})
target_link_libraries(${target} ${LIB})
