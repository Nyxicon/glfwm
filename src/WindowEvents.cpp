#include "WindowManager.hpp"
#include "glfwm/WindowEvents.hpp"

namespace nyx {

    void CreateWindowEvent::handle(WindowManager &manager) {
        manager.createNewWindow(this->application);
    }

    void DestroyWindow::handle(WindowManager &manager) {
        manager.destroyWindow(this->windowHandle);
    }

    void WindowTitle::handle(WindowManager &manager) {
        Window *w = manager.getWindow(this->windowHandle);
        if(w != nullptr) glfwSetWindowTitle(w->getGlfwWindow(), newTitle.c_str());
    }

} // namespace