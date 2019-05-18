set(target ${LIBUTILS})

file(GLOB hrcs 		"*.h")
file(GLOB srcs    	"*.c")

include_directories(${OPENCL_INCLUDE_DIRS})
message(STATUS "OpenCL INC - ${OPENCL_INCLUDE_DIRS}") 
add_library(${target} STATIC ${srcs} ${hrcs})
target_link_libraries(${target} ${OPENCL_LIBRARIES})
