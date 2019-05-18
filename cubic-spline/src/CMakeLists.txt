set(target ${LIB})
set(target_python spline)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(PythonInterp 3 REQUIRED)
find_package(PythonLibs 3 REQUIRED)
find_package(NumPy REQUIRED)
find_package(Boost COMPONENTS REQUIRED python3)
# find_package(BoostNumPy REQUIRED)

include_directories(${Boost_INCLUDE_DIRS} ${PYTHON_INCLUDE_DIRS} ${NUMPY_INCLUDE_DIRS})

file(GLOB hrcs  "${ROOT}/*.h")
file(GLOB srcs  "*.cpp")

add_library(${target} ${hrcs} ${srcs})

python_add_module(${target_python} ${hrcs} pythonSpline.cxx)
#file(COPY {target_python} "{ROOT}/scripts")
set_property(TARGET ${target} PROPERTY POSITION_INDEPENDENT_CODE ON)

message(STATUS "python ${PYTHON_LIBRARIES}")
message(STATUS "boost ${Boost_LIBRARIES}")
message(STATUS "boostPy  ${NUMPY_INCLUDE_DIRS} ")

target_link_libraries(${target} ${MKL_LIBRARIES})
target_link_libraries(${target_python}
                        ${Boost_LIBRARIES}
                        ${PYTHON_LIBRARIES}
                        ${target})

