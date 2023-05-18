set(this Spark)
ucm_add_dirs(Src TO sources RECURSIVE)
add_library(${this} STATIC ${sources})
target_include_directories(${this} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/Src)