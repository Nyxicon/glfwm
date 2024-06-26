#ifndef GLFWM_APPLICATION_HPP
#define GLFWM_APPLICATION_HPP

#include <vector>
#include <memory>
#include "Config.hpp"
#include "WindowPlugin.hpp"

namespace nyx {

    class Application {
    public:
        const std::unique_ptr<WindowHandle> windowHandle;
        explicit Application(std::unique_ptr<WindowHandle> handle) : windowHandle(std::move(handle)) {}
        virtual void configure(Config &config) = 0;
        virtual void create() = 0;
        virtual void render(float dt, long frameTime) = 0;
        virtual void resize(int width, int height) = 0;
        virtual void dispose() = 0;
        virtual ~Application() = default;
    };

} // namespace

#endif //GLFWM_APPLICATION_HPP