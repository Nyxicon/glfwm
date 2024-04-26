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

    /*struct InitHints {
        bool useJoystickHatButtons = true;
        int anglePlatformType = GLFW_ANGLE_PLATFORM_TYPE_NONE;
        int platform = GLFW_ANY_PLATFORM;
        bool useCocoaChDirResources = true;
        bool useCocoaMenubar = true;
        bool useX11XcbVulkanSurface = true;
        bool disableWaylandLibdecor = false;
    };*/

    class GLFWM {
    public:
        GLFWM(GLFWM const &) = delete;
        void operator=(GLFWM const &) = delete;
        static void init();

        template<class T, typename... Args>
        static T &createWindow(Args &&...args) {
            if (!nyx::GLFWM::initialized) throw std::runtime_error("GLFWM::createWindow: GLFWM not initialized.");
            // Window class takes ownership of WindowHandle & Application pointer, moving would invalidate references
            WindowHandle *handle = WindowHandle::createNewWindowHandle();
            T *app = new T(handle, args...);
            GLFWM::pushWindowEvent<CreateWindowEvent>(handle, app);
            return *app;
        }

        template<class T, typename... Args>
        static T &createSharedWindow(const Application *sharedApp, Args &&...args) {
            if (!nyx::GLFWM::initialized) throw std::runtime_error("GLFWM::createSharedWindow: GLFWM not initialized.");
            // Window class takes ownership of WindowHandle & Application pointer, moving would invalidate references
            WindowHandle *handle = WindowHandle::createNewWindowHandle(sharedApp->windowHandle.groupId);
            T *app = new T(handle, args...);
            GLFWM::pushWindowEvent<CreateWindowEvent>(handle, app);
            return *app;
        }

        template<class T, typename... Args>
        static void pushWindowEvent(Args &&...args) {
            if (!nyx::GLFWM::initialized) throw std::runtime_error("GLFWM::pushWindowEvent: GLFWM not initialized.");
            GLFWM::pushWindowEventImpl(std::unique_ptr<T>(new T(args...)));
        }
        static void destroyWindow(WindowHandle &handle);
        static void pollEventsBlocking();
        static void terminate();

    private:
        static bool initialized;
        std::unique_ptr<WindowManager> windowManager;
        GLFWM() = default;
        ~GLFWM();
        static GLFWM &instance();
        static void pushWindowEventImpl(std::unique_ptr<WindowEvent> event);
    };

} // namespace

#endif //GLFWM_GLFWM_HPP