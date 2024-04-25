#ifndef GLFWM_WINDOWHANDLE_HPP
#define GLFWM_WINDOWHANDLE_HPP

#include <iostream>
#include <memory>
#include <mutex>

namespace nyx {

    // TODO: make it owned by window, application only gets reference ?
    class WindowHandle {
    public:
        const int windowId;
        const int groupId;
    private:
        friend class GLFWM; // allow only GLFWM to create instances of this
        static std::unique_ptr<WindowHandle> createNewWindowHandle(int groupId = -1) {
            static int windowIdCounter = 0;
            static int windowGroupIdCounter = 0;
            return std::unique_ptr<WindowHandle>(
                    new WindowHandle(windowIdCounter++, (groupId != -1) ? groupId : windowGroupIdCounter++)
            );
        }
        // terminate events require null handle
        friend class TerminateEvent;
        friend class InternalTerminateEvent;
        static WindowHandle &getNullWindowHandle() {
            // From C++11 onwards static initialization is thread safe
            // Access is thread safe since threads at most only read const values
            static WindowHandle empty(-1, -1);
            return empty;
        }
        explicit WindowHandle(int windowId, int groupId) : windowId(windowId), groupId(groupId) {}
    };

} // namespace

#endif //GLFWM_WINDOWHANDLE_HPP