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
        explicit WindowGroup(int i);
        void pushInternalWindowEvent(InternalWindowEvent *event);
        void addWindow(Window *window);
        void terminate();
        ~WindowGroup();
        bool isEmpty();
        Window *getWindow(WindowHandle &handle);

    private:
        moodycamel::ReaderWriterQueue<InternalWindowEvent *> internalWindowEventQueue; // TODO: use unique_ptr
        std::vector<Window *> windows; // TODO: use unique_ptr
        std::thread thread;
        std::atomic<bool> stopping;

        std::atomic<bool> empty; // used by WindowManager to check if group should be removed

        static void loop(WindowGroup &group);
    };

} // namespace

#endif //GLFWM_WINDOWGROUP_HPP