#ifndef GLFWM_WINDOW_HPP
#define GLFWM_WINDOW_HPP

#include <string>
#include <mutex>
#include <condition_variable>
// TODO: check if define is needed
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glfwm/Application.hpp"

namespace nyx { class WindowGroup; }

namespace nyx {

    class Window {
    public:
        Window(Application *app, WindowGroup &g, const std::string& title);
        void init();
        void render();
        ~Window();
        Application &getApplication();
        GLFWwindow *getGlfwWindow();
        KeyCallback *getKeyCallback();
        MouseCallback *getMouseCallback();

        std::condition_variable cv;
        std::mutex mutex;
        bool isTerminated() const {
            return terminated;
        }
        void terminate() {
            // TODO: add check to only terminate from thread that also renders it...
            this->terminated = true;
        }

        long lastFrameTime = 0;

    private:
        bool terminated = false;
        WindowGroup &group;
        Application *application;
        GLFWwindow *glfwWindow;

        WindowCallback *windowCallback;
        KeyCallback *keyCallback;
        MouseCallback * mouseCallback;
        JoystickCallback *joystickCallback;

        double previousTime = glfwGetTime();
        int frameCount = 0;

        void setWindowHints(Config &config);
        void createWindow(Config &config, const std::string &debugTitle);
        void registerCallbacks(Config &config);
    };

}

#endif //GLFWM_WINDOW_HPP