#ifndef GLFWM_WINDOW_HPP
#define GLFWM_WINDOW_HPP

#include <string>
#include <mutex>
#include <condition_variable>
#include "glfwm/Application.hpp"
#include "glfwm/KeyCallback.hpp"
#include "glfwm/MouseCallback.hpp"
#include "glfwm/JoystickCallback.hpp"
#include "glfwm/WindowCallback.hpp"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

namespace nyx { class WindowGroup; }

namespace nyx {

    class Window {
    public:
        Window(Application *app, WindowHandle *handle, WindowGroup &g);
        void init();
        void render();
        void scheduleTermination();
        bool isTerminated() const;
        ~Window();
        Application &getApplication();
        WindowHandle &getWindowHandle();
        GLFWwindow *getGlfwWindow();
        KeyCallback *getKeyCallback();
        MouseCallback *getMouseCallback();

        // move into get methods
        std::condition_variable cv;
        std::mutex mutex;
        long lastFrameTime = 0;

    private:
        const std::unique_ptr<Application> application; // if moved reference returned by GLFWM would be invalidated
        const std::unique_ptr<WindowHandle> windowHandle; // if moved reference stored by application is invalidated
        WindowGroup &group;

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

        int windowWidth, windowHeight;

        void setWindowHints(Config &config);
        void createWindow(Config &config);
        void registerCallbacks(Config &config);
    };

} // namespace

#endif //GLFWM_WINDOW_HPP