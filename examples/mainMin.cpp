#include "glfwm/GLFWM.hpp"
#include "glfwm/Application.hpp"

class Game : public nyx::Application {
public:
    using Application::Application;
    void configure(nyx::Config &config) override {}
    void create() override {}
    void render(float dt, long frameTime) override {}
    void resize(int width, int height) override {}
    void dispose() override {}
    ~Game() override = default;
};

int main() {
    nyx::GLFWM::init();
    Game &g = nyx::GLFWM::createWindow<Game>();
    nyx::GLFWM::createSharedWindow<Game>(&g);
    nyx::GLFWM::createWindow<Game>();
    nyx::GLFWM::pollEventsBlocking();
    return 0;
}