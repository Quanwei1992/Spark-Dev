cmake_minimum_required(VERSION 3.25)

include(Cmake/ucm/ucm.cmake)
include(Cmake/Utils.cmake)

set_property(GLOBAL PROPERTY USE_FOLDERS ON) 
set(CMAKE_CXX_STANDARD 17)

project(Spark)

add_subdirectory(Spark)
add_subdirectory(SandBox)