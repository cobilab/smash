cmake_minimum_required(VERSION 2.8.4)

SET(GCC_COVERAGE_COMPILE_FLAGS "-Wall -O2 -msse2 -w")
SET( CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )
IF(UNIX)
 link_libraries(m)
ENDIF(UNIX)

project (SMASH) 
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
SET(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
SET(CMAKE_BUILD_TYPE Release)

add_executable (SMASH smash.c mem.c common.c reverse.c filters.c segment.c context.c paint.c defs.h) 

