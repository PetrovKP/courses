set(target ${APPLICATION})

file(GLOB srcs "main.cpp")

find_package(OpenGL)
find_package(GLUT)

include_directories(
        ${OPENGL_INCLUDE_DIR}
        ${GLUT_INCLUDE_DIR}
)

add_executable(${target} ${srcs})

target_link_libraries( ${target}
                       ${LIBRARY}
                       m
                       ${OPENGL_gl_LIBRARY}
                       ${GLUT_glut_LIBRARY}
                       )