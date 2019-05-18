set(target ${LIB})

file(GLOB hrcs "${ROOT}/.h")
file(GLOB srcs "*.c")

add_library(${target} STATIC ${srcs} ${hrcs})

add_definitions(-DKERNEL_CL_PATH="${CMAKE_CURRENT_LIST_DIR}/gpu/gemm.cl")

include_directories(${OPENCL_INCLUDE_DIRS})

add_subdirectory(gpu)
target_link_libraries(${target} ${LIBGPU})
target_link_libraries(${target} ${LIBUTILS})
target_link_libraries(${target} ${OPENCL_LIBRARIES})
