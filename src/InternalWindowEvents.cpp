#include <iostream>
#include "WindowGroup.hpp"
#include "InternalWindowEvents.hpp"

namespace nyx {

    void InternalCreateWindowEvent::handle(nyx::WindowGroup &group) {
        group.addWindow(this->window);
    }

    void InternalKeyEvent::handle(WindowGroup &group) {
        KeyCallback *keyCallback = group.getWindow(windowHandle)->getKeyCallback();
        if(keyCallback != nullptr) keyCallback->keyCallback(key, scancode, action, mods);
    }

    void InternalCharEvent::handle(WindowGroup &group) {
        KeyCallback *keyCallback = group.getWindow(windowHandle)->getKeyCallback();
        if(keyCallback != nullptr) keyCallback->charCallback(codepoint);
    }

    void InternalCharModsEvent::handle(WindowGroup &group) {
        KeyCallback *keyCallback = group.getWindow(windowHandle)->getKeyCallback();
        if(keyCallback != nullptr) keyCallback->charModsCallback(codepoint, mods);
    }

    void InternalMouseButtonEvent::handle(WindowGroup &group) {
        MouseCallback *mouseCallback = group.getWindow(windowHandle)->getMouseCallback();
        if(mouseCallback != nullptr) mouseCallback->mouseButtonCallback(button, action, mods);
    }

    void InternalMousePosEvent::handle(WindowGroup &group) {
        MouseCallback *mouseCallback = group.getWindow(windowHandle)->getMouseCallback();
        if(mouseCallback != nullptr) mouseCallback->cursorPosCallback(xpos, ypos);
    }

    void InternalMouseScrollEvent::handle(WindowGroup &group) {
        MouseCallback *mouseCallback = group.getWindow(windowHandle)->getMouseCallback();
        if(mouseCallback != nullptr) mouseCallback->scrollCallback(xoffset, yoffset);
    }

    void InternalMouseEnterEvent::handle(WindowGroup &group) {
        MouseCallback *mouseCallback = group.getWindow(windowHandle)->getMouseCallback();
        if(mouseCallback != nullptr) mouseCallback->cursorEnterCallback(entered);
    }

} // namespace