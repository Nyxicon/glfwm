#ifndef GLFWM_WINDOWGROUP_HPP
#define GLFWM_WINDOWGROUP_HPP

#include <vector>
#include <thread>
#include <map>
#include "readerwriterqueue.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "InternalWindowEvents.hpp"
#include "Window.hpp"

namespace nyx {

    class WindowGroup {
    public:
        const int id;
        explicit WindowGroup(int i) : id(i) {}
        void pushInternalWindowEvent(InternalWindowEvent *event);
        void startThread(); // TODO: maybe rename !?
        void addWindow(Window *window);
        void joinThread(); // TODO: rename to something like terminate !?
        bool empty();
        Window *getWindow(WindowHandle &handle);

    private:
        // TODO: refactor to use std::unique_ptr<InternalWindowEvent>
        moodycamel::ReaderWriterQueue<InternalWindowEvent *> internalWindowEventQueue;
        std::vector<Window *> windows;
        //std::vector<std::unique_ptr<Window>> windows;
        std::thread thread;
        static void loop(WindowGroup &group);
    };

} // namespace

#endif //GLFWM_WINDOWGROUP_HPP