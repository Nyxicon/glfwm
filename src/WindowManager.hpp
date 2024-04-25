#ifndef GLFWM_WINDOWMANAGER_HPP
#define GLFWM_WINDOWMANAGER_HPP

#include <vector>
#include <map>
#include "glfwm/WindowHandle.hpp"
#include "glfwm/WindowEvents.hpp"
#include "WindowGroup.hpp"
#include "concurrentqueue.h"

namespace nyx {

    class WindowManager {
    public:
        WindowManager() : stopping(false) {}
        void pushWindowEvent(std::unique_ptr<WindowEvent> event);
        void createNewWindow(Application *app);
        void destroyWindow(WindowHandle &handle);
        void pollEventsBlocking();
        void terminate();
        ~WindowManager() = default;
        Window* getWindow(WindowHandle &handle);

    private:
        moodycamel::ConcurrentQueue<std::unique_ptr<WindowEvent>> windowEventQueue;
        std::map<int, std::unique_ptr<WindowGroup>> windowGroups;
        std::atomic<bool> stopping;
    };

    struct TerminateEvent : public WindowEvent {
        explicit TerminateEvent() : WindowEvent(WindowHandle::getNullWindowHandle()) {}
        void handle(WindowManager &manager) override { manager.terminate(); }
    };

} // namespace

#endif //GLFWM_WINDOWMANAGER_HPP