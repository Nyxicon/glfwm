#ifndef GLFWM_WINDOWHANDLE_HPP
#define GLFWM_WINDOWHANDLE_HPP

#include <iostream>
#include <memory>
#include <mutex>

namespace nyx {

    class WindowHandle {
    public:
        const int windowId;
        const int groupId;

    private:
        friend class GLFWM; // allow only GLFWM to create instances of this
        static WindowHandle *createNewWindowHandle(int groupId = -1) {
            static int windowIdCounter = 0;
            static int windowGroupIdCounter = 0;
            return new WindowHandle(windowIdCounter++, (groupId != -1) ? groupId : windowGroupIdCounter++);
        }
        // terminate events require null handle
        friend class TerminateEvent;
        friend class InternalTerminateEvent;
        static WindowHandle &getNullWindowHandle() {
            static WindowHandle empty(-1, -1);
            return empty;
        }
        explicit WindowHandle(int windowId, int groupId) : windowId(windowId), groupId(groupId) {}
    };

} // namespace

#endif //GLFWM_WINDOWHANDLE_HPP