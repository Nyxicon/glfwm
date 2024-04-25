#include "WindowManager.hpp"
#include "glfwm/WindowEvents.hpp"

namespace nyx {

    void CreateWindowEvent::handle(WindowManager &manager) {
        manager.createNewWindow(this->windowHandle, this->application);
    }

    void DestroyWindow::handle(WindowManager &manager) {
        manager.destroyWindow(this->windowHandle);
    }

    void ShowWindow::handle(WindowManager &manager) {
        Window *w = manager.getWindow(this->windowHandle);
        if (w != nullptr) glfwShowWindow(w->getGlfwWindow());
    }

    void HideWindow::handle(WindowManager &manager) {
        Window *w = manager.getWindow(this->windowHandle);
        if (w != nullptr) glfwHideWindow(w->getGlfwWindow());
    }

    void WindowTitle::handle(WindowManager &manager) {
        Window *w = manager.getWindow(this->windowHandle);
        if (w != nullptr) glfwSetWindowTitle(w->getGlfwWindow(), newTitle.c_str());
    }

    void WindowPosition::handle(WindowManager &manager) {
        Window *w = manager.getWindow(this->windowHandle);
        if (w != nullptr) glfwSetWindowPos(w->getGlfwWindow(), this->xpos, this->ypos);
    }

    void WindowSize::handle(WindowManager &manager) {
        Window *w = manager.getWindow(this->windowHandle);
        if (w != nullptr) glfwSetWindowSize(w->getGlfwWindow(), this->width, this->height);
    }

    void WindowSizeLimits::handle(WindowManager &manager) {
        Window *w = manager.getWindow(this->windowHandle);
        if (w != nullptr)
            glfwSetWindowSizeLimits(w->getGlfwWindow(), this->minwidth, this->minheight,
                                    this->maxwidth, this->maxheight);
    }

    void WindowAspectRatio::handle(WindowManager &manager) {
        Window *w = manager.getWindow(this->windowHandle);
        if (w != nullptr) glfwSetWindowAspectRatio(w->getGlfwWindow(), this->numer, this->denom);
    }

    void WindowOpacity::handle(WindowManager &manager) {
        Window *w = manager.getWindow(this->windowHandle);
        if (w != nullptr) glfwSetWindowOpacity(w->getGlfwWindow(), this->opacity);
    }

    void IconifyWindow::handle(WindowManager &manager) {
        Window *w = manager.getWindow(this->windowHandle);
        if (w != nullptr) glfwIconifyWindow(w->getGlfwWindow());
    }

    void MaximizeWindow::handle(WindowManager &manager) {
        Window *w = manager.getWindow(this->windowHandle);
        if (w != nullptr) glfwMaximizeWindow(w->getGlfwWindow());
    }

    void RestoreWindow::handle(WindowManager &manager) {
        Window *w = manager.getWindow(this->windowHandle);
        if (w != nullptr) glfwRestoreWindow(w->getGlfwWindow());
    }

} // namespace