set(target ${LIBGPU})

file(GLOB src "*.cu")

cuda_include_directories(${ROOT})
cuda_add_library(${target} ${src})
target_link_libraries(${target} ${CUDA_LIBRARIES})
