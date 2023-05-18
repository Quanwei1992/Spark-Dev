cmake_minimum_required(VERSION 3.25)

include(Cmake/ucm/ucm.cmake)

set_property(GLOBAL PROPERTY USE_FOLDERS ON) 
set(CMAKE_CXX_STANDARD 20)
ucm_set_runtime(STATIC)

project(Spark)

add_subdirectory(Spark)
add_subdirectory(SandBox)
