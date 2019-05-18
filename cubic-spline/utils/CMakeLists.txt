set(target ${LIBUTILS})

file(GLOB hrcs 		"*.h")
file(GLOB srcs    	"*.c")

add_library(${target} STATIC ${srcs} ${hrcs})
