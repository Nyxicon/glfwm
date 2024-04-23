#ifndef GLFWM_WINDOWCALLBACK_HPP
#define GLFWM_WINDOWCALLBACK_HPP

namespace nyx {

    class WindowCallback {
    public:
        virtual void position(int xpos, int ypos) = 0;
        virtual void size(int width, int height) = 0;
        virtual void refresh() = 0;
        virtual void focus(int focused) = 0;
        virtual void iconify(int iconified) = 0;
        virtual void maximize(int maximized) = 0;
        virtual void content_scale(float xscale, float yscale) = 0;
        virtual bool close() = 0;
        virtual ~WindowCallback() = default;
    };

} // namespace

#endif //GLFWM_WINDOWCALLBACK_HPP