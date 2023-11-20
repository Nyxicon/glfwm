#ifndef GLFWM_WINDOWHANDLE_HPP
#define GLFWM_WINDOWHANDLE_HPP

namespace nyx {

    // TODO: make unique pointer owned by Window, Application gets a reference
    class WindowHandle {
    public:
        const int windowId;
        const int groupId;
    private:
        friend class GLFWM;
        static WindowHandle* createNewWindowHandle(int groupId = -1) {
            static int windowIdCounter = 0;
            static int windowGroupIdCounter = 0;
            return new WindowHandle(windowIdCounter++, (groupId != -1) ? groupId : windowGroupIdCounter++);
        }
        explicit WindowHandle(int windowId, int groupId) : windowId(windowId), groupId(groupId) {}
    };

}

#endif //GLFWM_WINDOWHANDLE_HPP