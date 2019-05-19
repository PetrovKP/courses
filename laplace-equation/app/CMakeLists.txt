set(target ${APP})

file(GLOB  src "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(${target} ${src})
target_link_libraries(${target} ${LIBRARY_SERIAL})
target_link_libraries(${target} ${LIBRARY_OMP})
#target_link_libraries(${target} ${LIBRARY_TBB})
target_link_libraries(${target} ${MPI_LIBRARIES})