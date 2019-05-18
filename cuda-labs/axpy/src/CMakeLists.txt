set(target ${LIB})

file(GLOB hrcs "${ROOT}/.h")
file(GLOB srcs "*.c")

add_library(${target} STATIC ${srcs} ${hrcs})

if (${CUDA_FOUND})
	add_subdirectory(gpu)
    target_link_libraries(${target} ${LIBGPU})
endif()
target_link_libraries(${target} ${LIBUTILS})