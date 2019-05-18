set(target ${LIBRARY})

file(GLOB hrcs "${CMAKE_HOME_DIRECTORY}/include/*.h")
file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

find_package(GLEW REQUIRED)
link_libraries(${GLEW_LIBRARIES})

add_library( ${target} STATIC ${srcs} ${hrcs})


target_link_libraries( ${target}
                       m
                       ${OPENGL_gl_LIBRARY}
                       ${GLUT_glut_LIBRARY}
                       )