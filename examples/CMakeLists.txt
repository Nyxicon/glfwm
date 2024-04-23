add_subdirectory(libs/imgui-1.90.5)
target_link_libraries(imgui-1.90.5 glfw)

# fetch glad
FetchContent_Declare(glad GIT_REPOSITORY https://github.com/Dav1dde/glad.git GIT_TAG v0.1.36)
set(GLAD_PROFILE "core"	CACHE STRING "OpenGL profile")
set(GLAD_GENERATOR "c"	CACHE STRING "Language to generate the binding for")
#set(GLAD_EXTENSIONS "" CACHE STRING "Path to extensions file or comma separated list of extensions, if missing all extensions are included" FORCE)
FetchContent_MakeAvailable(glad)

add_executable(MinimalEngineExample
        #mainMin.cpp
        main.cpp
        DearImGuiPlugin.hpp
        GladPlugin.hpp
)
target_link_libraries(MinimalEngineExample PRIVATE glfwm)
target_link_libraries(MinimalEngineExample PUBLIC imgui-1.90.5)
target_link_libraries(MinimalEngineExample PUBLIC glad)