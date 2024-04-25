#ifndef GLFWM_APPLICATION_HPP
#define GLFWM_APPLICATION_HPP

#include <memory>
#include <vector>
#include "Config.hpp"
#include "WindowHandle.hpp"
#include "WindowPlugin.hpp"

namespace nyx {

    class Application {
    public:
        WindowHandle &windowHandle; // TODO: make const
        explicit Application(WindowHandle &handle) : windowHandle(handle) {}
        virtual void configure(Config &config) = 0;
        virtual void create() = 0;
        virtual void render(double dt, long frameTime) = 0;
        virtual void resize(int width, int height) = 0;
        virtual void dispose() = 0;
        virtual ~Application() = default;
    };

} // namespace

#endif //GLFWM_APPLICATION_HPP