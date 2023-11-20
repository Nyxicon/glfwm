#ifndef GLFWM_WINDOWMANAGER_HPP
#define GLFWM_WINDOWMANAGER_HPP

#include <vector>
#include <map>
#include "glfwm/WindowHandle.hpp"
#include "WindowGroup.hpp"
#include "concurrentqueue.h"

namespace nyx { class WindowEvent; }

namespace nyx {

    class WindowManager {
    public:
        WindowManager() = default;
        void createNewWindow(Application *app);
        void recreateWindow(WindowHandle &handle);
        void destroyWindow(WindowHandle &handle);
        void pushWindowEvent(std::unique_ptr<WindowEvent> event);
        void pollEventsBlocking();
        void terminate();
        ~WindowManager() = default;
        Window* getWindow(WindowHandle &handle);

    private:
        moodycamel::ConcurrentQueue<std::unique_ptr<WindowEvent>> windowEventQueue;
        std::map<int, std::unique_ptr<WindowGroup>> windowGroups;
        // TODO: add running var
    };

}

#endif //GLFWM_WINDOWMANAGER_HPP