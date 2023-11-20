#ifndef GLFWM_PLUGIN_HPP
#define GLFWM_PLUGIN_HPP

// InitPlugin
// RenderPlugin

class Plugin {
public:
    virtual void init() = 0;
    virtual void onCreateWindow(/* handle, glfwWindow */) = 0;
    virtual void preRenderWindow() = 0;
    virtual void postRenderWindow() = 0;
    virtual void dispose() = 0;
    virtual ~Plugin() = default;
    virtual Plugin* newInstance() = 0;
};

#endif //GLFWM_PLUGIN_HPP