set(target ${APPLICATION})

file(GLOB srcs "main.cpp")

add_executable(${target} ${srcs})

find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)

target_link_libraries(${target}
                      ${LIBRARY}
                      ${OPENGL_LIBRARIES}
                      ${GLUT_LIBRARY})
