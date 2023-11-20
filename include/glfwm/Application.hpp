#ifndef GLFWM_APPLICATION_HPP
#define GLFWM_APPLICATION_HPP

#include <vector>
#include "Config.hpp"
#include "Plugin.hpp"

namespace nyx {

    class Application {
    public:
        WindowHandle &window; // TODO: rename to handle
        explicit Application(WindowHandle &handle) : window(handle) {}
        virtual void configure(Config &config) = 0;
        virtual void create() = 0;
        virtual void render(float dt, long frameTime) = 0;
        virtual void resize(int width, int height) = 0;
        virtual ~Application() = default;
    };

}

#endif //GLFWM_APPLICATION_HPP