set(this SandBox)
ucm_add_dirs(Src TO sources RECURSIVE)
add_executable(${this} ${sources})
target_link_libraries(${this} PRIVATE Spark)