#include <string>
#include <iostream>
#include <mutex>
#include "glfwm/WindowEvents.hpp"
#include "WindowManager.hpp"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "Window.hpp"

namespace nyx {

    void WindowManager::pushWindowEvent(std::unique_ptr<WindowEvent> event) {
        // only accept events while not stopping or to destroy windows when stopping
        if (!stopping.load() || dynamic_cast<DestroyWindow*>(event.get()))
            this->windowEventQueue.enqueue(std::move(event));
        //glfwPostEmptyEvent();
    }

    void WindowManager::createNewWindow(Application *app) {
        // catch CreateWindowEvents that were submitted after TerminateEvent but before queue closed
        if(this->stopping.load()) return;

        auto &handle = app->windowHandle;

        // create & start group if not found
        if (this->windowGroups.find(handle->groupId) == this->windowGroups.end()) {
            this->windowGroups.insert({handle->groupId, std::unique_ptr<WindowGroup>(new WindowGroup(handle->groupId))});
            //this->windowGroups.at(handle->groupId)->startThread();
        }
        auto &group = this->windowGroups.at(handle->groupId);

        // create the window & push to wind group
        auto *window = new Window(app, *group);
        group->pushInternalWindowEvent(new InternalCreateWindowEvent(*handle, window));
    }

    void WindowManager::destroyWindow(WindowHandle &handle) {
        int groupId = handle.groupId;

        // check that group exists
        if (windowGroups.find(groupId) == windowGroups.end()) return;

        // get requested window from group
        auto *window = windowGroups.at(handle.groupId)->getWindow(handle);
        if (window == nullptr) return;

        // inform window & group that window should be removed
        glfwSetWindowShouldClose(window->getGlfwWindow(), GLFW_TRUE);

        // wait until window is removed from group and ready to be destroyed
        std::unique_lock<std::mutex> lk(window->mutex);
        window->cv.wait(lk, [&window] { return window->isTerminated(); });

        glfwDestroyWindow(window->getGlfwWindow()); // must be called from the main thread

        delete window; // TODO: could probably be replaced with a unique_ptr ?

        // destroy group if empty
        if (this->windowGroups.at(groupId)->isEmpty()) {
            this->windowGroups.erase(groupId);
            if(this->windowGroups.empty()) this->stopping.store(true);
        }
    }

    void WindowManager::pollEventsBlocking() {
        while (true) {
            glfwPollEvents(); //glfwWaitEvents(); waiting can possibly be more resource saving

            // handle all queued window events
            std::unique_ptr<WindowEvent> p;
            while (windowEventQueue.try_dequeue(p)) p->handle(*this);

            // check if glfwm should terminate
            if (this->windowGroups.empty() && this->stopping.load()) break;
        }

        // clear out possibly remaining events
        std::unique_ptr<WindowEvent> p;
        while (windowEventQueue.try_dequeue(p));
    }

    void WindowManager::terminate() {
        // stop accepting new events
        this->stopping.store(true);

        // push events to all window groups to terminate
        for(auto &groupItr : this->windowGroups)
            groupItr.second->pushInternalWindowEvent(new InternalTerminateEvent());
    }

    Window *WindowManager::getWindow(WindowHandle &handle) {
        if (this->windowGroups.find(handle.groupId) == this->windowGroups.end()) return nullptr;
        return this->windowGroups.at(handle.groupId)->getWindow(handle);
    }

} // namespace