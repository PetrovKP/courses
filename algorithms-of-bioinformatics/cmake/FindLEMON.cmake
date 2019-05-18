# Find the COIN-OR::Lemon includes and library
# This module defines
#  LEMON_INCLUDE_DIR: where to find lemon/
#  LEMON_LIBRARIES: the libraries needed to use lemon
#  LEMON_FOUND: if false, do not try to use lemon

IF(LEMON_INCLUDE_DIR AND LEMON_LIBRARIES)
    SET(LEMON_FOUND TRUE)
ELSE(LEMON_INCLUDE_DIR AND LEMON_LIBRARIES)
    FIND_PATH(LEMON_INCLUDE_DIR lemon/euler.h lemon/config.h
        /usr/include
        /usr/local/include
        ${CMAKE_HOME_DIRECTORY}/lemon-1.3.1/
        ${CMAKE_HOME_DIRECTORY}/lemon-1.3.1/_build/
    )
    FIND_LIBRARY(LEMON_LIBRARIES NAMES emon libemon
        PATHS
        /usr/lib
        /usr/local/lib
        /usr/lib64
        /usr/local/lib64
        /nfs/inn/proj/numerics1/Users/kpetrov/home/algorithms_of_bioinformatics/lemon-1.3.1/_build/lemon
    )

    IF(LEMON_INCLUDE_DIR AND LEMON_LIBRARIES)
        SET(LEMON_FOUND TRUE)
        MESSAGE(STATUS "Found COIN-OR::Lemon: ${LEMON_INCLUDE_DIR}, ${LEMON_LIBRARIES}")
    ELSE(LEMON_INCLUDE_DIR AND LEMON_LIBRARIES)
        SET(LEMON_FOUND FALSE)
        MESSAGE(STATUS "COIN-OR::Lemon not found.")
    ENDIF(LEMON_INCLUDE_DIR AND LEMON_LIBRARIES)

    MARK_AS_ADVANCED(LEMON_INCLUDE_DIR LEMON_LIBRARIES)
ENDIF(LEMON_INCLUDE_DIR AND LEMON_LIBRARIES)
