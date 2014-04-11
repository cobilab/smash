cmake_minimum_required(VERSION 2.8.4)

#SET(GCC_COVERAGE_COMPILE_FLAGS "-fstrict-aliasing -ffast-math -msse2")
#SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

project (SMASH) 
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
SET(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable (smash mem.c common.c context.c defs.h) 



