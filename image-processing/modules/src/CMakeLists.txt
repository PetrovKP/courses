set(target ${LIB})

file(GLOB SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB HEADERS_FILES "./../include/*.h")

add_library(${target} STATIC ${SOURCE_FILES} ${HEADERS_FILES})

target_link_libraries(${target} ${OpenCV_LIBS})