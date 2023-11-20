
#include <string>
#include <iostream>
#include<mutex>
#include "glfwm/WindowEvents.hpp"

#include "WindowManager.hpp"
#include "Window.hpp"

#include "GLFW/glfw3.h"

namespace nyx {

    void WindowManager::createNewWindow(Application *app) {
        auto handle = app->window;

        // create group if not found
        auto initialize = bool(false);
        if (this->windowGroups.find(handle.groupId) == this->windowGroups.end()) {
            this->windowGroups.insert({handle.groupId, std::unique_ptr<WindowGroup>(new WindowGroup(handle.groupId))});
            initialize = true;
        }

        auto &group = this->windowGroups.at(handle.groupId);;

        // create window | TODO: remove and replace by config.title
        auto title = std::string();
        title.append("id: ").append(std::to_string(handle.windowId));
        title.append(" group: ").append(std::to_string(handle.groupId));
        auto *window = new Window(app, *group, title);

        // push window to group & start thread if group is new
        group->pushInternalWindowEvent(new InternalCreateWindowEvent(handle, window));
        if (initialize) group->startThread();
    }

    void WindowManager::recreateWindow(WindowHandle &handle) {
        throw std::runtime_error("WindowManager::recreateWindow: Function not yet implemented !");
        // TODO: implement recreating window
        // TODO: 1. create new window with shared context
        // TODO: 2. destroy old window
        // TODO: 3. finish new window
        // Window* window = windowGroups.at(handle.groupId)->getWindow(handle);
        // Application& app = window->getApplication();
    }

    void WindowManager::destroyWindow(WindowHandle &handle) {
        auto window = windowGroups.at(handle.groupId)->getWindow(handle);
        if (window == nullptr) throw std::runtime_error("WindowManager::destroyWindow: Window is nullptr.");

        // inform window & group that window should be removed
        glfwSetWindowShouldClose(window->getGlfwWindow(), GLFW_TRUE);

        // wait until window is removed from group and ready to be destroyed
        std::unique_lock<std::mutex> lk(window->mutex);
        window->cv.wait(lk, [&window] { return window->isTerminated(); });

        glfwDestroyWindow(window->getGlfwWindow());

        // destroy group | TODO: improve destroy process
        if (this->windowGroups.at(handle.groupId)->empty()) {
            this->windowGroups.at(handle.groupId)->joinThread();
            this->windowGroups.erase(handle.groupId);
        }

        delete &handle; // TODO: remove through unique_ptr
    }

    void WindowManager::pushWindowEvent(std::unique_ptr<WindowEvent> event) {
        this->windowEventQueue.enqueue(std::move(event));
        //glfwPostEmptyEvent();
    }

    void WindowManager::pollEventsBlocking() {
        // TODO: change glfwWaitEvents to end of loop
        // TODO: make while loop depend on bool var
        //       -> don't consume events once bool is false
        while (true) {
            //glfwWaitEvents();
            glfwPollEvents();

            // handle all queued window events
            std::unique_ptr<WindowEvent> p;
            while (windowEventQueue.try_dequeue(p)) p->handle(*this);

            // check if glfwm should terminate
            if (this->windowGroups.empty()) break; // TODO: should it actually terminate ?
        }
        // TODO: terminate everything
        // TODO: also allow terminate to stop poll-loop
    }

    void WindowManager::terminate() {
        std::cout << "terminate application" << std::endl;
        // TODO: stop accepting events before destroying
    }

    Window *WindowManager::getWindow(WindowHandle &handle) {
        if (this->windowGroups.find(handle.groupId) == this->windowGroups.end()) return nullptr;
        return this->windowGroups.at(handle.groupId)->getWindow(handle);
    }

}