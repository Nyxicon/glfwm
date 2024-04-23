#ifndef GLFWM_GLADPLUGIN_HPP
#define GLFWM_GLADPLUGIN_HPP

#include "glfwm/WindowPlugin.hpp"
#include "glad/glad.h"

class GladPlugin : public nyx::WindowPlugin {
public:
    void interceptConfig(nyx::Config &config) override {

    }

    void onWindowCreated(nyx::Config &config, GLFWwindow *window) override {

    }

    void onWindowInit(GLFWwindow *window) override {
        gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
    }

    void preRenderWindow() override {
        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void postRenderWindow() override {

    }

    void dispose() override {

    }

    ~GladPlugin() override = default;
};

#endif //GLFWM_GLADPLUGIN_HPP
