cmake_minimum_required(VERSION 3.25)

include(Cmake/ucm/ucm.cmake)

set(CMAKE_CXX_STANDARD 17)
ucm_set_runtime(STATIC)

project(Spark)

add_subdirectory(Spark)
add_subdirectory(SandBox)
