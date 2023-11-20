#ifndef GLFWM_WINDOWCALLBACKEVENTS_HPP
#define GLFWM_WINDOWCALLBACKEVENTS_HPP

#include "Window.hpp"
namespace nyx { class WindowGroup; }

namespace nyx {

    struct InternalWindowEvent {
        WindowHandle &windowHandle;
        explicit InternalWindowEvent(WindowHandle &handle) : windowHandle(handle) {}
        virtual void handle(WindowGroup &group) = 0;
        virtual ~InternalWindowEvent() = default;
    };

    struct InternalCreateWindowEvent : public InternalWindowEvent {
        Window *window;
        explicit InternalCreateWindowEvent(WindowHandle &handle, Window *w) : InternalWindowEvent(handle), window(w) {}
        void handle(WindowGroup &group) override;
    };

    struct InternalKeyEvent : public InternalWindowEvent {
        int key, scancode, action, mods;
        explicit InternalKeyEvent(WindowHandle &handle, int key, int scancode, int action, int mods)
                : InternalWindowEvent(handle), key(key), scancode(scancode), action(action), mods(mods) {}
        void handle(WindowGroup &group) override;
    };

    struct InternalCharEvent : public InternalWindowEvent {
        unsigned int codepoint;
        explicit InternalCharEvent(WindowHandle &handle, unsigned int codepoint)
                : InternalWindowEvent(handle), codepoint(codepoint) {}
        void handle(WindowGroup &group) override;
    };

    struct InternalCharModsEvent : public InternalWindowEvent {
        unsigned int codepoint;
        int mods;
        explicit InternalCharModsEvent(WindowHandle &handle, unsigned int codepoint, int mods)
                : InternalWindowEvent(handle), codepoint(codepoint), mods(mods) {}
        void handle(WindowGroup &group) override;
    };

    struct InternalMouseButtonEvent : public InternalWindowEvent {
        int button, action, mods;
        explicit InternalMouseButtonEvent(WindowHandle &handle, int button, int action, int mods)
                : InternalWindowEvent(handle), button(button), action(action), mods(mods) {}
        void handle(WindowGroup &group) override;
    };

    struct InternalMousePosEvent : public InternalWindowEvent {
        double xpos, ypos;
        explicit InternalMousePosEvent(WindowHandle &handle, double xpos, double ypos)
                : InternalWindowEvent(handle), xpos(xpos), ypos(ypos) {}
        void handle(WindowGroup &group) override;
    };

    struct InternalMouseScrollEvent : public InternalWindowEvent {
        double xoffset, yoffset;
        explicit InternalMouseScrollEvent(WindowHandle &handle, double xoffset, double yoffset)
                : InternalWindowEvent(handle), xoffset(xoffset), yoffset(yoffset) {}
        void handle(WindowGroup &group) override;
    };

    struct InternalMouseEnterEvent : public InternalWindowEvent {
        int entered;
        explicit InternalMouseEnterEvent(WindowHandle &handle, int entered)
                : InternalWindowEvent(handle), entered(entered) {}
        void handle(WindowGroup &group) override;
    };

}

#endif //GLFWM_WINDOWCALLBACKEVENTS_HPP