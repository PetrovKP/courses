set(target ${LIBRARY})

file(GLOB hrcs "${CMAKE_HOME_DIRECTORY}/include/*.h")
file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

find_package(OpenGL)
find_package(GLUT)
find_package(GLEW REQUIRED)

if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    link_libraries(${GLEW_LIBRARIES})
endif()

include_directories(
        ${OPENGL_INCLUDE_DIR}
        ${GLUT_INCLUDE_DIR}
)

add_library( ${target} STATIC ${srcs} ${hrcs})


target_link_libraries( ${target}
                       m
                       ${OPENGL_gl_LIBRARY}
                       ${GLUT_glut_LIBRARY}
                       )