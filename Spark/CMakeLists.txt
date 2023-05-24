
add_subdirectory(Extern)
add_subdirectory(Libs)

set(this Spark)

ucm_add_dirs(Src TO sources RECURSIVE)
add_library(${this} STATIC ${sources})
target_include_directories(${this} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/Src)
target_link_libraries(${this} PRIVATE glfw glad assimp)
target_link_libraries(${this} PUBLIC spdlog glm)
set_property(TARGET ${this} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")