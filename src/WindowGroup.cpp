#include <iostream>
#include "Window.hpp"
#include "WindowGroup.hpp"

#include "glfwm/GLFWM.hpp"
#include "glad/glad.h"

#include "glfwm/WindowEvents.hpp"

namespace nyx {

    void WindowGroup::pushInternalWindowEvent(InternalWindowEvent *event) {
        this->internalWindowEventQueue.enqueue(event);
    }

    void WindowGroup::startThread() {
        this->thread = std::thread(WindowGroup::loop, std::ref(*this));
    }

    void WindowGroup::addWindow(Window *window) {
        // Called from InternalCreateWindowEvent -> thread safe
        glfwMakeContextCurrent(window->getGlfwWindow());
        window->init();
        // TODO: Plugin::onCreateWindow
        glfwMakeContextCurrent(nullptr);
        this->windows.push_back(window);
    }

    // TODO: rename render, create new loop method move while(true) to it
    void WindowGroup::loop(WindowGroup &group) {
        // TODO: Plugin::init
        while (true) {
            // handle all queued internal window events



            InternalWindowEvent *event = nullptr;
            while(group.internalWindowEventQueue.try_dequeue(event)) {
                event->handle(group);
                delete event;
                event = nullptr;
            }

            // update all windows
            for(int i = 0; i < group.windows.size(); i++) {
                auto window = group.windows.at(i);

                if(glfwWindowShouldClose(window->getGlfwWindow())) {
                    // remove window from group & push destroy event to WindowManager

                    group.windows.erase(group.windows.begin() + i);
                    i--;

                    std::unique_lock<std::mutex> lk(window->mutex);
                    window->terminate();

                    // TODO: call dispose from original thread ?

                    lk.unlock();
                    window->cv.notify_one();

                    continue; // skip rendering for this window
                }

                glfwMakeContextCurrent(window->getGlfwWindow());


                auto start = std::chrono::high_resolution_clock::now();

                window->render();

                glfwSwapBuffers(window->getGlfwWindow());

                auto stop = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
                window->lastFrameTime = duration.count();
            }

            // if no windows remain -> terminate this thread
            // TODO: stop new events from being pushed to this thread, consume all remaining events & handle them to
            //       avoid memory being leaked by unhandled events
            if(group.windows.empty()) break;
        }
        // TODO: delete all windows
        glfwMakeContextCurrent(nullptr);
    }

    void WindowGroup::joinThread() {
        this->thread.join();
    }

    bool WindowGroup::empty() {
        return this->windows.empty();
    }

    Window *WindowGroup::getWindow(WindowHandle &handle) {
        for(auto window : windows)
            if(window->getApplication().window.windowId == handle.windowId)
                return window;
        return nullptr;
    }

}