#include <iostream>
#include "WindowGroup.hpp"
#include "Window.hpp"
#include "glfwm/GLFWM.hpp"
#include "glfwm/WindowEvents.hpp"

namespace nyx {

    WindowGroup::WindowGroup(int i) : id(i), stopping(false), empty(false) {
        this->thread = std::thread(WindowGroup::loop, std::ref(*this));
    }

    void WindowGroup::pushInternalWindowEvent(InternalWindowEvent *event) {
        if (!stopping.load())
            this->internalWindowEventQueue.enqueue(event);
    }

    void WindowGroup::addWindow(Window *window) {
        // Called from InternalCreateWindowEvent -> thread safe
        if (!stopping.load()) {
            window->init();
            this->windows.push_back(window);
        } else {
            // TODO: window must be terminated
            // by creating the window right here
            // -> as benefit the application constructor can be thread safe with gl context
        }
    }

    // TODO: rename render, create new loop method move while(true) to it
    void WindowGroup::loop(WindowGroup &group) {
        while (true) {
            // handle all queued internal window events
            InternalWindowEvent *event = nullptr;
            while (group.internalWindowEventQueue.try_dequeue(event)) {
                event->handle(group);
                delete event;
                event = nullptr;
            }

            // update all windows
            for (int i = 0; i < group.windows.size(); i++) {
                auto window = group.windows.at(i);

                if (glfwWindowShouldClose(window->getGlfwWindow())) {
                    group.windows.erase(group.windows.begin() + i);
                    i--;

                    // TODO: maybe call something like destroy & flag ready to delete idk
                    window->scheduleTermination();

                    if (group.windows.empty()) {
                        group.stopping.store(true); // if last window was destroyed, terminate group
                        group.empty.store(true); // inform WindowManager that this was the last window -> destroy group
                    }

                    continue; // skip rendering for this window
                }

                // render window
                glfwMakeContextCurrent(window->getGlfwWindow());
                auto start = std::chrono::high_resolution_clock::now();
                window->render();
                auto stop = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
                window->lastFrameTime = duration.count();
                glfwSwapBuffers(window->getGlfwWindow());
            }

            // if no windows remain -> terminate this thread
            if (group.windows.empty() && group.stopping.load()) break;
        }

        // TODO: clear possible remaining events from queue
        // disallow pushing new events once group is stopping
        // maybe use atomic bool running after all ?

        glfwMakeContextCurrent(nullptr);
    }

    void WindowGroup::terminate() {
        // stop accepting new events
        this->stopping.store(true);

        // push destroy event for every window
        for (auto &window: windows)
            nyx::GLFWM::pushWindowEvent<DestroyWindow>(*window->getApplication().windowHandle);
    }

    WindowGroup::~WindowGroup() {
        this->thread.join();
    }

    bool WindowGroup::isEmpty() {
        // TODO: this is not thread safe !!!
        return this->windows.empty();
    }

    Window *WindowGroup::getWindow(WindowHandle &handle) {
        for (auto window: windows)
            if (window->getApplication().windowHandle->windowId == handle.windowId)
                return window;
        return nullptr;
    }

} // namespace