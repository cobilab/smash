cmake_minimum_required(VERSION 2.8.4)

SET(GCC_COVERAGE_COMPILE_FLAGS "-Wall -02 -fstrict-aliasing -ffast-math -msse2 -w")
SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )
IF(UNIX)
 link_libraries(m)
ENDIF(UNIX)

project (SMASH) 
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
SET(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable (smash smash.c mem.c common.c reverse.c filters.c segment.c context.c paint.c defs.h) 

