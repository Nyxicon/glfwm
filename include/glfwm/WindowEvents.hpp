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
        // don't use unique_ptr, else reference returned in GLFWM would be invalidated on std::move
        WindowHandle *windowHandle; // overshadow parent windowHandle
        Application *application;
        CreateWindowEvent(WindowHandle *handle, Application *app)
                : WindowEvent(*handle), windowHandle(handle), application(app) {}
        void handle(WindowManager &manager) override;
    };

    struct DestroyWindow : public WindowEvent {
        explicit DestroyWindow(WindowHandle &handle) : WindowEvent(handle) {}
        void handle(WindowManager &manager) override;
    };

    struct ShowWindow : public WindowEvent {
        explicit ShowWindow(WindowHandle &handle) : WindowEvent(handle) {}
        void handle(WindowManager &manager) override;
    };

    struct HideWindow : public WindowEvent {
        explicit HideWindow(WindowHandle &handle) : WindowEvent(handle) {}
        void handle(WindowManager &manager) override;
    };

    struct WindowTitle : public WindowEvent {
        std::string newTitle;
        explicit WindowTitle(WindowHandle &handle, std::string title)
                : WindowEvent(handle), newTitle(std::move(title)) {}
        void handle(WindowManager &manager) override;
    };

    // WindowIcon glfwSetWindowIcon (int width, int height, unsigned char *pixels)
    // new signature requires an array of GLFWImage*

    struct WindowPosition : public WindowEvent {
        int xpos, ypos;
        explicit WindowPosition(WindowHandle &handle, int x, int y)
                : WindowEvent(handle), xpos(x), ypos(y){}
        void handle(WindowManager &manager) override;
    };

    struct WindowSize : public WindowEvent {
        int width, height;
        explicit WindowSize(WindowHandle &handle, int w, int h)
                : WindowEvent(handle), width(w), height(h){}
        void handle(WindowManager &manager) override;
    };

    struct WindowSizeLimits : public WindowEvent {
        int minwidth, minheight, maxwidth, maxheight;
        explicit WindowSizeLimits(WindowHandle &handle, int minw, int minh, int maxw, int maxh)
                : WindowEvent(handle), minwidth(minw), minheight(minh), maxwidth(maxw), maxheight(maxh) {}
        void handle(WindowManager &manager) override;
    };

    struct WindowAspectRatio : public WindowEvent {
        int numer, denom;
        explicit WindowAspectRatio(WindowHandle &handle, int n, int d) : WindowEvent(handle), numer(n), denom(d) {}
        void handle(WindowManager &manager) override;
    };

    struct WindowOpacity : public WindowEvent {
        float opacity;
        explicit WindowOpacity(WindowHandle &handle, float o) : WindowEvent(handle), opacity(o) {}
        void handle(WindowManager &manager) override;
    };

    struct IconifyWindow : public WindowEvent {
        explicit IconifyWindow(WindowHandle &handle) : WindowEvent(handle){}
        void handle(WindowManager &manager) override;
    };

    struct MaximizeWindow : public WindowEvent {
        explicit MaximizeWindow(WindowHandle &handle) : WindowEvent(handle){}
        void handle(WindowManager &manager) override;
    };

    struct RestoreWindow : public WindowEvent {
        explicit RestoreWindow(WindowHandle &handle) : WindowEvent(handle){}
        void handle(WindowManager &manager) override;
    };

    /* Exposed functions through Window class: (main thread only)
     - glfwFocusWindow (-)
     - glfwRequestWindowAttention (-)
     - glfwSetWindowMonitor (GLFWmonitor* mh, int xpos, int ypos, int width, int height, int refreshRate) | switch windowed & fullscreen
     - glfwSetWindowAttrib (int attrib, int value)
       -> GLFW_RESIZABLE
       -> GLFW_DECORATED
       -> GLFW_FLOATING
       -> GLFW_AUTO_ICONIFY
       -> GLFW_FOCUS_ON_SHOW
       -> GLFW_MOUSE_PASSTHROUGH (glfw 3.4)
       */

    // TODO: maybe store in window class to fetch later ? maybe even inside WindowHandle ?
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