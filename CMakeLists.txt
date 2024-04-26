cmake_minimum_required(VERSION 3.24)
project(glfwm)

set(CMAKE_CXX_STANDARD 11)

include(FetchContent)

# fetch glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(glfw URL https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.zip)
FetchContent_MakeAvailable(glfw)

# fetch reader-writer
FetchContent_Declare(readerwriterqueue
        GIT_REPOSITORY    https://github.com/cameron314/readerwriterqueue
        GIT_TAG           master)
FetchContent_MakeAvailable(readerwriterqueue)

# fetch concurrent-queue
FetchContent_Declare(concurrentqueue URL https://github.com/cameron314/concurrentqueue/archive/refs/tags/v1.0.4.zip)
FetchContent_MakeAvailable(concurrentqueue)

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
        include/glfwm/WindowPlugin.hpp

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
)

target_include_directories(glfwm
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/include>
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(glfwm PUBLIC glfw)
target_link_libraries(glfwm PRIVATE concurrentqueue)
target_link_libraries(glfwm PRIVATE readerwriterqueue)

# example
#add_subdirectory(examples)