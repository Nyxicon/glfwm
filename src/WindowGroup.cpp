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
            this->internalWindowEventQueue.enqueue(std::unique_ptr<InternalWindowEvent>(event));
    }

    void WindowGroup::addWindow(Window *window) {
        if (!stopping.load()) {
            window->init();
            this->windows.push_back(window);
        } else delete window; // group is already stopping -> just destroy window
    }

    void WindowGroup::loop(WindowGroup &group) {
        while (true) {
            // handle all queued internal window events
            std::unique_ptr<InternalWindowEvent> event;
            while (group.internalWindowEventQueue.try_dequeue(event)) event->handle(group);

            // update all windows
            for (int i = 0; i < group.windows.size(); i++) {
                auto window = group.windows.at(i);

                // check if window should close
                if (glfwWindowShouldClose(window->getGlfwWindow())) {
                    group.windows.erase(group.windows.begin() + i);
                    i--;
                    window->destroyAndFlagTerminated();
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
        glfwMakeContextCurrent(nullptr);

        // clear possibly remaining events
        std::unique_ptr<InternalWindowEvent> event;
        while (group.internalWindowEventQueue.try_dequeue(event)) event->handle(group);
    }

    void WindowGroup::terminate() {
        // stop accepting new events
        this->stopping.store(true);

        // push destroy event for every window
        for (auto &window: windows)
            nyx::GLFWM::pushWindowEvent<DestroyWindow>(window->getWindowHandle());
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
            if (window->getWindowHandle().windowId == handle.windowId)
                return window;
        return nullptr;
    }

} // namespace