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
        Window(std::unique_ptr<Application> app, WindowGroup &g);
        void init();
        void render();
        void scheduleTermination();
        ~Window();
        Application &getApplication();
        GLFWwindow *getGlfwWindow();
        KeyCallback *getKeyCallback();
        MouseCallback *getMouseCallback();
        bool isTerminated() const;

        // move into get methods
        std::condition_variable cv;
        std::mutex mutex;
        long lastFrameTime = 0;

    private:
        const std::unique_ptr<Application> application;
        WindowGroup &group;

        // TODO: change to reference
        // if application implements a listener, we would double delete it once as Application once as ...Callback
        WindowCallback *windowCallback;
        KeyCallback *keyCallback;
        MouseCallback * mouseCallback;
        JoystickCallback *joystickCallback;

        std::vector<WindowPlugin*> plugins; // TODO: make unique_ptr
        GLFWwindow *glfwWindow;
        bool terminated = false;
        double previousTime = glfwGetTime();
        int frameCount = 0;

        void setWindowHints(Config &config);
        void createWindow(Config &config);
        void registerCallbacks(Config &config);
    };

} // namespace

#endif //GLFWM_WINDOW_HPP