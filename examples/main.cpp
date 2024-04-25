#include "glfwm/Application.hpp"
#include "glfwm/GLFWM.hpp"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "DearImGuiPlugin.hpp"
#include "GladPlugin.hpp"

class Game : public nyx::Application, nyx::KeyCallback {
public:
    explicit Game(nyx::WindowHandle &handle, float r, float g, float b)
            : Application(handle), shouldRender(true), red(r), green(g), blue(b) {
        // constructor does not have active context
    }

    void configure(nyx::Config &config) override {
        //config.title = "Window";
        config.keyCallback = this;
        config.plugins.push_back(new GladPlugin());
        config.plugins.push_back(new DearImGuiPlugin());
    }

    void create() override {} // called after constructor with active context

    void render(float dt, long frameTime) override {
        if (shouldRender) glClearColor(red, green, blue, 1.0f);
        else glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui::ShowDemoWindow();
    }

    void resize(int width, int height) override {}

    void dispose() override {} // called before destructor with active context

    ~Game() override = default; // destructor does not have active context

    int i = 213;

private:
    bool shouldRender;
    float red, green, blue;

    void keyCallback(int key, int scancode, int action, int mods) override {
        if (action == GLFW_PRESS) shouldRender = false;
        if (action == GLFW_RELEASE) {
            shouldRender = true;
            if (key == GLFW_KEY_O) nyx::GLFWM::pushWindowEvent<nyx::DestroyWindow>(this->windowHandle);
            if (key == GLFW_KEY_T) nyx::GLFWM::pushWindowEvent<nyx::WindowTitle>(this->windowHandle, "Test");
            if (key == GLFW_KEY_ESCAPE) nyx::GLFWM::terminate();
        }
    }

    void charCallback(unsigned int codepoint) override {

    }

    void charModsCallback(unsigned int codepoint, int mods) override {

    }
};

int main() {
    nyx::GLFWM::init();
    Game &g = nyx::GLFWM::createWindow<Game>(1.0f, 0.3f, 0.3f);
    nyx::GLFWM::createSharedWindow<Game>(&g, 0.3f, 1.0f, 0.3f);
    nyx::GLFWM::createWindow<Game>(0.3f, 0.3f, 1.0f);
    nyx::GLFWM::pollEventsBlocking();
    return 0;
}