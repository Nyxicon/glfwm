#ifndef GLFWM_WINDOWCALLBACK_HPP
#define GLFWM_WINDOWCALLBACK_HPP

namespace nyx {

    class WindowCallback {
    public:
        virtual bool close() = 0;
        virtual ~WindowCallback() = default;
    };

} // namespace

#endif //GLFWM_WINDOWCALLBACK_HPP