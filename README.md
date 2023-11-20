# GLFWM
- - -
Minimalistic C++11 GLFW Multithreading Window Manager

## Features
- - -
- Object-oriented abstraction of the GLFW API
- Easy window configuration
- Creating a window with one function call
- Multiple windows rendering on different threads
- Multiple windows rendering on the same thread as group
- Calling GLFW functions from any thread (normally must be called from the main thread)
- GLFW callbacks dispatched from the thread rendering the window (instead of main thread)
- Minimal overhead and max performance

**NOTE:** This library is still in development and may slightly change.

## Usage
- - -
```cpp
#include "glfwm/GLFWM.hpp"
#include "glfwm/Application.hpp"

class Game : public nyx::Application {
public:
    using Application::Application;
    void configure(nyx::Config &config) override {}
    void create() override {}
    void render(float dt) override {}
    void resize(int width, int height) override {}
    ~Game() override = default;
};

int main() {
    nyx::GLFWM::init();
    Game &g = nyx::GLFWM::createWindow<Game>();
    nyx::GLFWM::createSharedWindow<Game>(&g);
    nyx::GLFWM::createWindow<Game>();
    nyx::GLFWM::pollEventsBlocking();
    return 0;
}
```
**Note:** ```nyx::Engine::init();``` and ```nyx::Engine::pollEventsBlocking();``` must be called from the main thread.
(GLFW restriction)

## GLFWM
- - -
The goal of this library is to provide a thin layer on top of GLFW to make creating and managing multiple windows on 
multiple threads easier.

## TODO's
- - -
### Known Issue
Sorted in order of descending importance
- [ ] Plugins (to add functionality to window(-groups) e.g. glad/glew & ImGui)
- [ ] Doxygen comments
- [ ] Further testing on different platforms (especially Windows & macOS)
- [ ] Automated testing
- [ ] (Config-) Support for windows with vulkan context
- [ ] Proper joystick/gamepad support
- [ ] Recreating windows
- [ ] WindowGroup executed on main thread
- [ ] Multiple windows sharing one context
- [ ] Window dependencies

If you're on Windows and using V-Sync, the frames are distributed evenly across all windows in a group.

(e.g. one Window with 60 Fps, two Windows with 30 Fps each, etc.)

**Note:** This library is still in development and requirements might change.

## CMake
- - -
This project uses CMake as build system.
```CMake
include(FetchContent)

FetchContent_Declare(glfwm
GIT_REPOSITORY    https://github.com/Nyxicon/glfwm.git
GIT_TAG           master)
FetchContent_MakeAvailable(glfwm)

add_library(my_target main.cpp)
target_link_libraries(my_target PUBLIC glfwm)
```

## Tests
- - -
I'm currently testing the project on Ubuntu 22.04 and Windows 11.

On Ubuntu im using valgrind memcheck & massif to check for memory leaks.

## Documentation
- - -
Most functions still need to be commented !

Methods are documented in doxygen format.

## System requirements
- - -
- C++11 compliant Compiler
- > GLFW supports Windows XP and later and macOS 10.8 and later.  Linux and other Unix-like systems running the X Window 
  > System are supported even without a desktop environment or modern extensions, although some features require a 
  > running window or clipboard manager.  The OSMesa backend requires Mesa 6.3.
  > 
  > See the [compatibility guide](https://www.glfw.org/docs/latest/compat.html) in the documentation for more information.
  > 
  > (https://github.com/glfw/glfw, 25.03.2023)

## License
- - -
GLFWM is licensed under the MIT License, see LICENSE.txt for more information.