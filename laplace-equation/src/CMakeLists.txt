set(target ${LIBRARY_SERIAL})

set(srcs serial.c)
set(hrcs ../include/serial.h)

add_library(${target} STATIC ${hrcs} ${srcs})

set(target ${LIBRARY_OMP})

set(srcs parallel.c)
set(hrcs ../include/parallel.h)

add_library(${target} STATIC ${hrcs} ${srcs})

#set(target ${LIBRARY_TBB})
#
#set(srcs solutionTbb.cpp)
#set(hrcs ../include/solutionTbb.h)
#
#add_library(${target} STATIC ${hrcs} ${srcs})
#target_link_libraries(${target} ${TBB_LIBRARIES})
#target_compile_definitions(${target} PRIVATE ${TBB_DEFINITIONS})
