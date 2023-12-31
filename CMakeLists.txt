cmake_minimum_required(VERSION 3.24)
project(glfwm)

set(CMAKE_CXX_STANDARD 11)

include(FetchContent)

# glad
add_subdirectory(libs/glad)

# glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(glfw URL https://github.com/glfw/glfw/releases/download/3.3.8/glfw-3.3.8.zip)
FetchContent_MakeAvailable(glfw)

# reader-writer
FetchContent_Declare(readerwriterqueue
        GIT_REPOSITORY    https://github.com/cameron314/readerwriterqueue
        GIT_TAG           master)
FetchContent_MakeAvailable(readerwriterqueue)

# concurrent-queue
add_subdirectory(libs/concurrent-queue-1.0.3) # TODO: change to fetch content

#add_subdirectory(examples/libs/imgui-1.89.2)
#target_link_libraries(imgui-1.89.2 glfw)

add_library(glfwm
        include/glfwm/GLFWM.hpp
        include/glfwm/Config.hpp
        include/glfwm/Application.hpp
        include/glfwm/WindowHandle.hpp
        include/glfwm/WindowEvents.hpp
        include/glfwm/WindowCallback.hpp
        include/glfwm/KeyCallback.hpp
        include/glfwm/MouseCallback.hpp
        include/glfwm/JoystickCallback.hpp

        src/GLFWM.cpp
        src/WindowEvents.cpp
        src/Window.cpp
        src/Window.hpp
        src/WindowGroup.cpp
        src/WindowGroup.hpp
        src/WindowManager.cpp
        src/WindowManager.hpp
        src/InternalWindowEvents.cpp
        src/InternalWindowEvents.hpp
        include/glfwm/Plugin.hpp)

target_include_directories(glfwm
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/include>
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(glfwm PUBLIC glfw)
target_link_libraries(glfwm PRIVATE concurrentqueue)
target_link_libraries(glfwm PRIVATE readerwriterqueue)
target_link_libraries(glfwm PUBLIC glad) # TODO: change to plugin
#target_link_libraries(glfwm PUBLIC imgui-1.89.2)

# example
add_subdirectory(examples)