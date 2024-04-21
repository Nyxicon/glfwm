#ifndef GLFWM_WINDOWPLUGIN_HPP
#define GLFWM_WINDOWPLUGIN_HPP

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

namespace nyx { class Config; } // break Config - WindowPlugin dependency

namespace nyx {

    class WindowPlugin {
    public:
        virtual void interceptConfig(Config &config) = 0;
        virtual void onWindowCreated(nyx::Config &config, GLFWwindow *window) = 0;
        virtual void onWindowInit(GLFWwindow *window) = 0;
        virtual void preRenderWindow() = 0;
        virtual void postRenderWindow() = 0;
        virtual void dispose() = 0;
        virtual ~WindowPlugin() = default;
    };

} // namespace

#endif //GLFWM_WINDOWPLUGIN_HPP