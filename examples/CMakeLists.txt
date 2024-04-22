add_subdirectory(libs/imgui-1.90.5)
target_link_libraries(imgui-1.90.5 glfw)

add_executable(MinimalEngineExample
        #mainMin.cpp
        main.cpp
        DearImGuiPlugin.hpp

)
target_link_libraries(MinimalEngineExample PRIVATE glfwm)
target_link_libraries(MinimalEngineExample PUBLIC imgui-1.90.5)