#ifndef GLFWM_WINDOWEVENTS_HPP
#define GLFWM_WINDOWEVENTS_HPP

#include <utility>

#include "WindowHandle.hpp"
#include "Application.hpp"

namespace nyx { class WindowManager; }

namespace nyx {

    struct WindowEvent {
        WindowHandle &windowHandle;
        explicit WindowEvent(WindowHandle &handle) : windowHandle(handle) {}
        virtual void handle(WindowManager &manager) = 0;
        virtual ~WindowEvent() = default;
    };

    struct CreateWindowEvent : public WindowEvent {
        Application *application;
        CreateWindowEvent(WindowHandle &handle, Application *app) : WindowEvent(handle), application(app) {}
        void handle(WindowManager &manager) override;
    };

    struct DestroyWindow : public WindowEvent {
        explicit DestroyWindow(WindowHandle &handle) : WindowEvent(handle) {}
        void handle(WindowManager &manager) override;
    };

    struct WindowTitle : public WindowEvent {
        std::string newTitle;
        explicit WindowTitle(WindowHandle &handle, std::string title)
                : WindowEvent(handle), newTitle(std::move(title)) {}
        void handle(WindowManager &manager) override;
    };

    struct HideWindow : public WindowEvent {
        explicit HideWindow(WindowHandle &handle) : WindowEvent(handle) {}
        void handle(WindowManager &manager) override;
    };

    /* Exposed functions through Window class: (main thread only)
     x glfwSetWindowTitle (const char* title)
     - glfwSetWindowIcon (int width, int height, unsigned char *pixels)
     - glfwSetWindowPos (int xpos, int ypos)
     - glfwSetWindowSizeLimits (int minwidth, int minheight, int maxwidth, int maxheight)
     - glfwSetWindowAspectRatio (int numer, int denom)
     - glfwSetWindowSize (int width, int height)
     - glfwSetWindowOpacity (float opacity)
     - glfwIconifyWindow (-)
     - glfwRestoreWindow (-)
     - glfwMaximizeWindow (-)
     - glfwShowWindow (-)
     - glfwHideWindow (-)
     - glfwFocusWindow (-)
     - glfwRequestWindowAttention (-)
     - glfwSetWindowMonitor (GLFWmonitor* mh, int xpos, int ypos, int width, int height, int refreshRate) | switch windowed & fullscreen
     - glfwSetWindowAttrib (int attrib, int value)
       -> GLFW_DECORATED
       -> GLFW_RESIZABLE
       -> GLFW_FLOATING
       -> GLFW_AUTO_ICONIFY
       -> GLFW_FOCUS_ON_SHOW*/

    /*Window property get functions:
    - glfwGetWindowPos
    - glfwGetWindowSize
    - glfwGetFramebufferSize
    - glfwGetWindowFrameSize
    - glfwGetWindowContentScale
    - glfwGetWindowOpacity
    - glfwGetWindowMonitor
    - glfwGetWindowAttrib
    They are a bit more complicated to implement: Push request object to main thread, call the glfw function and push
    the rec data back to the specific app into a callback queue. Then on the thread of the app poll the queue and execute
    the callback if it is not empty.*/

} // namespace

#endif //GLFWM_WINDOWEVENTS_HPP