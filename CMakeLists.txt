cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(engine)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

add_subdirectory(externals)
add_subdirectory(code)