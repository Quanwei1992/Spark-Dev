set(this glfw)

ucm_add_files(
    "glfw/include/GLFW/glfw3.h"
    "glfw/include/GLFW/glfw3native.h"
    "glfw/src/context.c"
    "glfw/src/init.c"
    "glfw/src/input.c"
    "glfw/src/monitor.c"
    "glfw/src/vulkan.c"
    "glfw/src/window.c"
    # Windows Only
    "glfw/src/win32_platform.h"
    "glfw/src/win32_joystick.h"
    "glfw/src/wgl_context.h"   
    "glfw/src/egl_context.h" 
    "glfw/src/osmesa_context.h" 
    "glfw/src/win32_init.c" 
    "glfw/src/win32_joystick.c" 
    "glfw/src/win32_monitor.c" 
    "glfw/src/win32_time.c" 
    "glfw/src/win32_thread.c" 
    "glfw/src/win32_window.c" 
    "glfw/src/wgl_context.c" 
    "glfw/src/egl_context.c"      
    "glfw/src/osmesa_context.c"
TO sources FILTER_POP 1)

add_library(${this} STATIC ${sources})
target_include_directories(${this} PUBLIC glfw/include)
target_compile_definitions(${this} PUBLIC _GLFW_WIN32 _CRT_SECURE_NO_WARNINGS)
set_target_properties(${this} PROPERTIES FOLDER Extern)
set_property(TARGET ${this} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")