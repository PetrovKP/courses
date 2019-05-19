set(target ${APP})

set(SOURCE_FILES main.cpp)

add_executable(${target} ${SOURCE_FILES})

target_link_libraries(${target} ${OpenCV_LIBS})
target_link_libraries(${target} ${LIB})