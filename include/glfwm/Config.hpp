#ifndef GLFWM_CONFIG_HPP
#define GLFWM_CONFIG_HPP

#include <vector>
#include <string>
#include "WindowHandle.hpp"
#include "KeyCallback.hpp"
#include "MouseCallback.hpp"
#include "JoystickCallback.hpp"
#include "WindowCallback.hpp"
#include "WindowPlugin.hpp"

namespace nyx {

    class Config {
    public:
        std::string title;
        int width = 640;
        int height = 360;
        // TODO: add aspect ratio attribute's and set after window is created

        // callbacks
        WindowCallback *windowCallback = nullptr;
        KeyCallback *keyCallback = nullptr;
        MouseCallback *mouseCallback = nullptr;
        JoystickCallback *joystickCallback = nullptr;
        // TODO: maybe add bitset to disable specific callbacks for each handler ?

        // window
        bool resizeable = true;
        bool visible = true;
        bool decorated = true;
        bool focused = true;
        bool autoIconify = true;
        bool floating = false;
        bool maximized = false;
        bool centerCursor = false;
        bool focusOnShow = false;
        bool scaleToMonitor = false;

        bool vsync = true;
        int fps = 60;

        // framebuffer
        int redBits = 8;
        int greenBits = 8;
        int blueBits = 8;
        int alphaBits = 8;
        int depthBits = 8;
        int stencilBits = 8;
        int accumRedBits = 0;
        int accumGreenBits = 0;
        int accumBlueBits = 0;
        int accumAlphaBits = 0;
        bool transparent = false;
        bool doubleBuffered = true;

        int samples = 2;

        std::vector<WindowPlugin*> plugins;

    private:
        friend class Window;
        Config() = default;
    };

} // namespace

#endif //GLFWM_CONFIG_HPP