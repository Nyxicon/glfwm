#ifndef GLFWM_MOUSECALLBACK_HPP
#define GLFWM_MOUSECALLBACK_HPP

namespace nyx {

    class MouseCallback {
    public:
        virtual void mouseButtonCallback(int button, int action, int mods) = 0;
        virtual void cursorPosCallback(double xpos, double ypos) = 0;
        virtual void scrollCallback(double xoffset, double yoffset) = 0;
        virtual void cursorEnterCallback(int entered) = 0;
        virtual ~MouseCallback() = default;
    };

} // namespace

#endif //GLFWM_MOUSECALLBACK_HPP