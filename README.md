# GLFWM
Minimalistic C++11 GLFW Multithreading Window Manager

## Features
- C++11 object-oriented abstraction of the GLFW API
- Easy window configurations
- Creating a window with one function call
- Multiple windows rendering on different threads
- Multiple windows rendering on the same thread as group
- Thread safe calling of GLFW functions (normally must be called from the main thread)
- Thread safe GLFW callbacks dispatched from the thread rendering the window (instead of main thread)
- WindowPlugins to add additional functionality on a per-window basis (e.g. Dear ImGui)

**NOTE:** This library is still in development and may slightly change.

## Usage
This creates two windows on one thread and another one on a different thread.
```cpp
#include "glfwm/GLFWM.hpp"
#include "glfwm/Application.hpp"

class Game : public nyx::Application {
public:
    using Application::Application;
    void configure(nyx::Config &config) override {}
    void create() override {}
    void render(float dt, long frameTime) override {}
    void resize(int width, int height) override {}
    void dispose() override {}
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

**Note:** When the constructor and destructor are called there is (currently) no active context yet. You can use 
create() and dispose() instead.

There's a more elaborate example using Dear ImGui in the ```examples``` directory.

## GLFWM Design
The goal of this library is to provide a thin layer on top of GLFW to make creating and managing multiple windows on 
multiple threads easier.
The focus is to strictly adhere to the given restrictions of glfw and while offering a higher level of thread safety.

Thread safety is mainly archived by using the incredibly excellent ReaderWriterQueue and ConcurrentQueue written by
cameron314.

### What it is not
The project is not a fully fledged out game engine, the primary goal is to make managing windows easier.<br>
Features like multiple scenes you can switch between or more elaborate multi input handling is purposefully not included
to keep this library as simple and generic as possible and allow the user to build his own engine/framework on top of
this project.

### API
The GLFWM class serves as a singleton encapsulating the core engine, allowing thread safe and easy access to all GLFW 
functionality at any time. <br>
This enables effortless window creation and destruction from any thread, at any time, without concerns regarding thread
safety or lifecycle management.

### Core
At the core of the project is the WindowManager class, which receives the API calls from the GLFWM Singleton and manages
multiple WindowGroups. Each WindowGroup consists of multiple Windows and creates a thread which renders all windows
belonging to this group. In addition, the WindowManager contains a multi-consumer, multi-produces ConcurrentQueue used
to push new WindowEvents to then be consumed and handled on the main thread.
Every WindowGroup consists of multiple windows rendered on the same thread and another ReaderWriterQueue to receive
events from the main thread.

Due to limitations windows are currently created and later destroyed on the main thread, which may differ from the
rendering thread. Consequently, the constructor and destructor of the application are called from the main thread and
have no active context. Alternatively you can use the ```create()``` and ```dispose()``` functions which are called
after the constructor and before the destructor with the active window context.

### Thread safety
Most times a GLFW event like input or a window resize is directly relevant to the data used on the rendering thread.
Due to GLFW restrictions these events must be polled on the main thread which means that the event may move a player or
resize a framebuffer while it is used by the rendering thread. This often leads to race conditions and hard to find 
bugs. At the same time there is almost no benefit receiving the event on a 'random' different thread (even if it is the
main thread).

GLFWM archives thread safety mainly due to the incredibly excellent ReaderWriterQueue and ConcurrentQueue written
by [cameron314](https://github.com/cameron314).

The ReaderWriterQueue and the ConcurrentQueue allow quick messages passing between the main thread receiving GLFW events
(through glfwPollEvents) and the rendering thread of the WindowGroup. This allows these events to be handled and called
from the same thread the window is rendered from.

Please keep in mind that this introduces a minimal amount of overhead for each GLFW call. However, in my opinion,
the advantage of being able to call GLFW functions from any thread and consume GLFW events directly in the corresponding
window/key/mouse callback without further need of synchronization far surpasses the slight performance impact. <br>

## TODO's
Sorted in order of descending importance
- [X] Plugins (to add functionality to window(-groups) e.g. glad/glew & ImGui)
- [ ] Window loop allowing to manually limit FPS
- [ ] WindowGroup executed on main thread
- [ ] Doxygen comments
- [ ] (Config-) Support for windows with vulkan context
- [ ] Proper joystick/gamepad support
- [ ] Multiple windows sharing one context
- [ ] Allow overwriting window handlers
- [ ] window-to-window user messages through WindowManager
- [ ] Recreating windows
- [ ] Window dependencies
- [ ] Further testing on different platforms (especially Windows & macOS)
- [ ] More/Better build systems
- [ ] Automated testing

**Note:** This library is still in development and requirements might change.

## Limitations
If you're on Windows and using the v-sync option, frames are distributed evenly across all windows in a group.
(e.g. one Window with 60 Fps, two Windows with 30 Fps each, etc.) This is most likely due to how glfwSwapBuffers is 
implemented on Windows.

## CMake
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
I'm currently testing the project on Ubuntu 22.04 and Windows 11.

On Ubuntu im using valgrind memcheck & massif to check for memory leaks.

## Documentation
Most functions still need to be commented !

Methods are documented in doxygen format.

## System requirements
- C++11 compliant Compiler
- > GLFW supports Windows XP and later and macOS 10.11 and later. Linux and other Unix-like systems running the X Window
  > System are supported even without a desktop environment or modern extensions, although some features require a
  > running window or clipboard manager. The OSMesa backend requires Mesa 6.3.
  > 
  > See the [compatibility guide](https://www.glfw.org/docs/latest/compat.html) in the documentation for more information.
  > 
  > (https://github.com/glfw/glfw, 21.04.2024)

## License
GLFWM is licensed under the MIT License, see LICENSE.txt for more information.