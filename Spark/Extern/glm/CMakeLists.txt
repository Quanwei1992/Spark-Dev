set(this glm)

ucm_add_dirs(glm/glm TO sources FILTER_POP 1)
ucm_add_files(glm/util/glm.natvis TO sources FILTER_POP 1)

add_library(${this} INTERFACE ${sources})
target_include_directories(${this} INTERFACE glm)
set_target_properties(${this} PROPERTIES FOLDER Extern)