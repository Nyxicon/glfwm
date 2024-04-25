#include "WindowManager.hpp"
#include "glfwm/GLFWM.hpp"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

namespace nyx {

    bool GLFWM::initialized = false;

    GLFWM &GLFWM::instance() {
        // Instantiated on first use & guaranteed to be destroyed.
        static GLFWM instance;
        return instance;
    }

    void GLFWM::init() {
        if (nyx::GLFWM::initialized) throw std::runtime_error("GLFWM::init: GLFWM already initialized.");
        // TODO: move into config
#ifdef __unix__
        glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11); // TODO: remove once wayland works reliably
#endif
        if (!glfwInit()) {
            const char *error;
            glfwGetError(&error);
            std::string s("GLFWM::init: GLFW failed to initialize.\n");
            s.append(error);
            throw std::runtime_error(s);
        }
        nyx::GLFWM::initialized = true;
        nyx::GLFWM::instance().windowManager = std::unique_ptr<WindowManager>(new WindowManager());
    }

    void GLFWM::destroyWindow(WindowHandle &handle) {
        if (!nyx::GLFWM::initialized) throw std::runtime_error("GLFWM::destroyWindow: GLFWM not initialized.");
        GLFWM::instance().windowManager->pushWindowEvent(std::unique_ptr<DestroyWindow>(new DestroyWindow(handle)));
    }

    void GLFWM::pollEventsBlocking() {
        if (!nyx::GLFWM::initialized) throw std::runtime_error("GLFWM::pollEventsBlocking: GLFWM not initialized.");
        GLFWM::instance().windowManager->pollEventsBlocking();
    }

    void GLFWM::terminate() {
        if (!nyx::GLFWM::initialized) throw std::runtime_error("Engine::terminate: GLFWM not initialized.");
        GLFWM::instance().windowManager->pushWindowEvent(
                std::unique_ptr<TerminateEvent>(new TerminateEvent())
        );
    }

    void GLFWM::pushWindowEventImpl(std::unique_ptr<WindowEvent> event) {
        GLFWM::instance().windowManager->pushWindowEvent(std::move(event));
    }

    GLFWM::~GLFWM() = default;

} // namespace