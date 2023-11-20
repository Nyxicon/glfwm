#ifndef GLFWM_KEYCALLBACK_HPP
#define GLFWM_KEYCALLBACK_HPP

namespace nyx {

    class KeyCallback {
    public:
        virtual void keyCallback(int key, int scancode, int action, int mods) = 0;
        virtual void charCallback(unsigned int codepoint) = 0;
        virtual void charModsCallback(unsigned int codepoint, int mods) = 0;
    };

}

#endif //GLFWM_KEYCALLBACK_HPP