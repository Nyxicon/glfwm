#ifndef GLFWM_GLFWM_HPP
#define GLFWM_GLFWM_HPP

#include <utility>
#include <memory>
#include <iostream>
#include <stdexcept>
#include "Application.hpp"
#include "WindowEvents.hpp"

namespace nyx { class WindowManager; } // hide engine implementation

namespace nyx {

    class GLFWM {
    public:
        GLFWM(GLFWM const &) = delete;
        void operator=(GLFWM const &) = delete;
        static void init();

        template<class T, typename... Args>
        static T &createWindow(Args &&...args) {
            if (!nyx::GLFWM::initialized) throw std::runtime_error("GLFWM::createWindow: GLFWM not initialized.");
            T *app = new T(WindowHandle::createNewWindowHandle(), args...);
            GLFWM::pushWindowEvent<CreateWindowEvent>(*app->windowHandle, app);
            return *app;
        }

        template<class T, typename... Args>
        static T &createSharedWindow(const Application *sharedApp, Args &&...args) {
            if (!nyx::GLFWM::initialized) throw std::runtime_error("GLFWM::createSharedWindow: GLFWM not initialized.");
            // WindowManager moves ownership to unique_ptr in window class
            T *app = new T(WindowHandle::createNewWindowHandle(sharedApp->windowHandle->groupId), args...);
            GLFWM::pushWindowEvent<CreateWindowEvent>(*app->windowHandle, app);
            return *app;
        }

        template<class T, typename... Args>
        static void pushWindowEvent(Args &&...args) { // TODO: rename
            if (!nyx::GLFWM::initialized) throw std::runtime_error("GLFWM::pushWindowEvent: GLFWM not initialized.");
            GLFWM::pushWindowEvent2(std::unique_ptr<T>(new T(args...)));
        }
        static void pushWindowEvent2(std::unique_ptr<WindowEvent> event);
        static void destroyWindow(WindowHandle &handle);
        static void pollEventsBlocking();
        static void terminate();

    private:
        static bool initialized;
        std::unique_ptr<WindowManager> windowManager;
        GLFWM() = default;
        ~GLFWM();
        static GLFWM &instance();
    };

} // namespace

#endif //GLFWM_GLFWM_HPP