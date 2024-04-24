#ifndef GLFWM_DEARIMGUIPLUGIN_HPP
#define GLFWM_DEARIMGUIPLUGIN_HPP

#include "glfwm/Config.hpp"
#include "glfwm/WindowPlugin.hpp"
#include "glfwm/KeyCallback.hpp"
#include "glfwm/MouseCallback.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// ====================================================================================================================
// +++ Important Info +++
// ====================================================================================================================
// If you want to use this plugin in multiple windows on different threads you MUST edit the global ImGui instance to be
// thread_local so that every thread can have its own global ImGui instance, else you would have to use locks to avoid
// race conditions.
//
// In imgui.cpp above "ImGuiContext*   GImGui = NULL;" there is a detailed description on how to achieve this editing
// imconfig.h. Since that sadly did not work for me I manually modified the following two lines:
// - imgui.cpp:
//    ImGuiContext* GImGui = NULL;           -> thread_local ImGuiContext* GImGui = NULL;
// - imgui_internal.h:
//    extern IMGUI_API ImGuiContext* GImGui; -> extern thread_local IMGUI_API ImGuiContext* GImGui;
//
// After this change it will work on Linux.

// On windows you have to additionally comment out the following line in imgui_impl_glfw.cpp:
// ::SetWindowLongPtrW((HWND)main_viewport->PlatformHandleRaw, GWLP_WNDPROC, (LONG_PTR)ImGui_ImplGlfw_WndProc);
// I still don't know why exactly the project is crashing when this callback is set...
// I'm still trying to find the reason and got a minimal example that is not crashing, so maybe i will slowly rewrite
// the project to find where exactly this problem is coming from ;-;
// ====================================================================================================================

// --------------------------------------------------------------------------------------------------------------------
// How to get ImGui working in a multi-threaded, multi-context environment
// --------------------------------------------------------------------------------------------------------------------
// ImGui uses a global context for storing most information.
//
// If you want to draw ImGui in multiple windows on the same thread you have to create one ImGui context window
// (OpenGl context?) and switch accordingly before calling any ImGui drawing functions.
// Extending this to draw in multiple windows on multiple threads you have to edit the global ImGui context to be
// thread_local to avoid race conditions between windows.
//
// As written in this issue (https://github.com/ocornut/imgui/issues/7155):
// "Namely, when you call glfwPollEvents() it will dispatch all GLFW events to the current context, indiscriminate of
// which window emitted them."
// "The solution is currently:"
// " - Call ImGui_ImplGlfw_InitForOpenGL() with install_callback=false."
// " - Install all 7 callbacks using GLFWwindow* window to your own function. In your function, change the imgui context
//     based on window, then call the corresponding ImGui_ImplGlfw_XXXX callback declared in imgui_impl_glfw.h"
// " - For ImGui_ImplGlfw_MonitorCallback() you need to call it for every context."
//
// To solve this we override the config key & mouse callbacks with these custom ImGuiCallbacks which will first push the
// inputs to ImGui, then checks if ImGui captured the event and if that was not the case, passes them forward to the
// original callbacks
// --------------------------------------------------------------------------------------------------------------------

// TODO: Missing ImGui_ImplGlfw_WindowFocusCallback();
class ImGuiCallbacks : public nyx::KeyCallback, public nyx::MouseCallback {
public:
    explicit ImGuiCallbacks(GLFWwindow *window, nyx::KeyCallback *keyCallback, nyx::MouseCallback *mouseCallback)
            : glfwWindow(window), context(nullptr), usrKeyCallback(keyCallback), usrMouseCallback(mouseCallback) {

    }

    void keyCallback(int key, int scancode, int action, int mods) override {
        //glfwMakeContextCurrent(glfwWindow);
        ImGui::SetCurrentContext(this->context);
        ImGui_ImplGlfw_KeyCallback(this->glfwWindow, key, scancode, action, mods);
        if (!ImGui::GetIO().WantCaptureKeyboard && usrKeyCallback != nullptr)
            this->usrKeyCallback->keyCallback(key, scancode, action, mods);
    }

    void charCallback(unsigned int codepoint) override {
        //glfwMakeContextCurrent(glfwWindow);
        ImGui::SetCurrentContext(this->context);
        ImGui_ImplGlfw_CharCallback(this->glfwWindow, codepoint);
        if (!ImGui::GetIO().WantCaptureKeyboard && this->usrKeyCallback != nullptr)
            this->usrKeyCallback->charCallback(codepoint);
    }

    void charModsCallback(unsigned int codepoint, int mods) override {
        if (usrKeyCallback != nullptr) this->usrKeyCallback->charModsCallback(codepoint, mods);
    }

    void mouseButtonCallback(int button, int action, int mods) override {
        //glfwMakeContextCurrent(glfwWindow);
        ImGui::SetCurrentContext(this->context);
        ImGui_ImplGlfw_MouseButtonCallback(this->glfwWindow, button, action, mods);
        if (!ImGui::GetIO().WantCaptureMouse && this->usrMouseCallback != nullptr)
            this->usrMouseCallback->mouseButtonCallback(button, action, mods);
    }

    void cursorPosCallback(double xpos, double ypos) override {
        //glfwMakeContextCurrent(glfwWindow);
        ImGui::SetCurrentContext(this->context);
        ImGui_ImplGlfw_CursorPosCallback(this->glfwWindow, xpos, ypos);
        if (!ImGui::GetIO().WantCaptureMouse && this->usrMouseCallback != nullptr)
            this->usrMouseCallback->cursorPosCallback(xpos, ypos);
    }

    void scrollCallback(double xoffset, double yoffset) override {
        //glfwMakeContextCurrent(glfwWindow);
        ImGui::SetCurrentContext(this->context);
        ImGui_ImplGlfw_ScrollCallback(this->glfwWindow, xoffset, yoffset);
        if (!ImGui::GetIO().WantCaptureMouse && this->usrMouseCallback != nullptr)
            this->usrMouseCallback->scrollCallback(xoffset, yoffset);
    }

    void cursorEnterCallback(int entered) override {
        //glfwMakeContextCurrent(glfwWindow);
        ImGui::SetCurrentContext(this->context);
        ImGui_ImplGlfw_CursorEnterCallback(this->glfwWindow, entered);
        if (!ImGui::GetIO().WantCaptureMouse && this->usrMouseCallback != nullptr)
            this->usrMouseCallback->cursorEnterCallback(entered);
    }

public:
    GLFWwindow *glfwWindow;
    ImGuiContext *context;
    nyx::KeyCallback *usrKeyCallback;
    nyx::MouseCallback *usrMouseCallback;
};

class DearImGuiPlugin : public nyx::WindowPlugin {
public:
    DearImGuiPlugin() {
        this->context = nullptr;
        this->glfwWindow = nullptr;
        this->imGuiCallbacks = nullptr;
    }

    void interceptConfig(nyx::Config &config) override {

    }

    void onWindowCreated(nyx::Config &config, GLFWwindow *window) override {
        this->glfwWindow = window;
        this->imGuiCallbacks = new ImGuiCallbacks(glfwWindow, config.keyCallback, config.mouseCallback);
        config.keyCallback = this->imGuiCallbacks;
        config.mouseCallback = this->imGuiCallbacks;
    }

    void onWindowInit(GLFWwindow *window) override {
        this->context = ImGui::CreateContext();
        ImGui::SetCurrentContext(this->context);
        ImGuiIO &io = ImGui::GetIO();
        (void) io;
        io.IniFilename = nullptr;
        ImGui::StyleColorsClassic();
        // https://github.com/ocornut/imgui/issues/7155
        ImGui_ImplGlfw_InitForOpenGL(this->glfwWindow, false); // setting true effectively breaks Dear ImGui
        ImGui_ImplOpenGL3_Init("#version 450");
        this->imGuiCallbacks->context = this->context;
    }

    void preRenderWindow() override {
        ImGui::SetCurrentContext(this->context);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void postRenderWindow() override {
        ImGui::EndFrame();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void dispose() override {
        glfwMakeContextCurrent(glfwWindow);
        ImGui::SetCurrentContext(this->context);
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext(this->context);
        delete this->imGuiCallbacks;
    }

    ~DearImGuiPlugin() override = default;

private:
    GLFWwindow *glfwWindow;
    ImGuiContext *context;
    ImGuiCallbacks *imGuiCallbacks;
};

#endif //GLFWM_DEARIMGUIPLUGIN_HPP