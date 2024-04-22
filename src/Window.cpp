#include <stdexcept>

#include "Window.hpp"
#include "WindowGroup.hpp"

#include "glad/glad.h"
#include "glfwm/WindowEvents.hpp"
#include "glfwm/GLFWM.hpp"

namespace nyx {

    Window::Window(std::unique_ptr<Application> app, WindowGroup &g)
            : application(std::move(app)), group(g), glfwWindow(nullptr) {
        Config config;
        this->application->configure(config);
        this->windowWidth = config.width;
        this->windowHeight = config.height;

        // set default debug title
        if (config.title.empty())
            config.title = "Window [" + std::to_string(application->windowHandle->windowId) + "|" +
                           std::to_string(application->windowHandle->groupId) + "]";

        // let plugins alter the config if needed
        this->plugins = std::move(config.plugins);
        for (auto &p: this->plugins) p->interceptConfig(config);

        // configure & create glfw window
        this->setWindowHints(config);
        this->createWindow(config);
        glfwSetWindowUserPointer(glfwWindow, this);

        for (auto &p: this->plugins) p->onWindowCreated(config, this->glfwWindow);

        // register all key, mouse and window callbacks
        this->windowCallback = config.windowCallback;
        this->keyCallback = config.keyCallback;
        this->mouseCallback = config.mouseCallback;
        this->joystickCallback = config.joystickCallback;
        this->registerCallbacks(config);
    }

    void Window::init() {
        glfwMakeContextCurrent(glfwWindow);
        glfwSwapInterval(1); // TODO: get vsync from config
        // TODO: remove glad -> inject custom loader glad/glew through plugin
        gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

        for (auto &p: this->plugins) p->onWindowInit(this->glfwWindow);

        application->create();
        application->resize(windowWidth, windowHeight);
        glfwMakeContextCurrent(nullptr);
        // TODO: push a "show window event" (see Window::setWindowHints)
        // only show window once its done
    }

    void Window::setWindowHints(Config &config) {
        glfwDefaultWindowHints();

        // TODO: if window is windowed -> make invisible and make visible later
        //       else if fullscreen -> ignore and set normally

        //struct _GLFWwndconfig
        //{
        glfwWindowHint(GLFW_RESIZABLE, config.resizeable);
        glfwWindowHint(GLFW_VISIBLE, config.visible);
        glfwWindowHint(GLFW_DECORATED, config.decorated);
        glfwWindowHint(GLFW_FOCUSED, config.focused);
        glfwWindowHint(GLFW_AUTO_ICONIFY, config.autoIconify);
        glfwWindowHint(GLFW_FLOATING, config.floating);
        glfwWindowHint(GLFW_CENTER_CURSOR, config.maximized);
        glfwWindowHint(GLFW_CENTER_CURSOR, config.centerCursor);
        glfwWindowHint(GLFW_FOCUS_ON_SHOW, config.focusOnShow);
        glfwWindowHint(GLFW_SCALE_TO_MONITOR, config.scaleToMonitor);
        //    struct {
        //        GLFWbool  retina;
        //        char      frameName[256];
        //    } ns;
        //    struct {
        //        char      className[256];
        //        char      instanceName[256];
        //    } x11;
        //};

        // case GLFW_COCOA_RETINA_FRAMEBUFFER: _glfw.hints.window.ns.retina = value ? GLFW_TRUE : GLFW_FALSE;
        // The default is to use full Retina resolution framebuffers
        //_glfw.hints.window.ns.retina = GLFW_TRUE;
        // glfwWindowHintString
        // - GLFW_COCOA_FRAME_NAME
        // - GLFW_X11_CLASS_NAME
        // - GLFW_X11_INSTANCE_NAME

        // The default is 24 bits of color, 24 bits of depth and 8 bits of stencil, double buffered
        // struct _GLFWfbconfig
        //{

        glfwWindowHint(GLFW_RED_BITS, config.redBits);
        glfwWindowHint(GLFW_BLUE_BITS, config.greenBits);
        glfwWindowHint(GLFW_GREEN_BITS, config.blueBits);
        glfwWindowHint(GLFW_ALPHA_BITS, config.alphaBits);
        glfwWindowHint(GLFW_DEPTH_BITS, config.depthBits);
        glfwWindowHint(GLFW_STENCIL_BITS, config.stencilBits);
        glfwWindowHint(GLFW_ACCUM_RED_BITS, config.accumRedBits);
        glfwWindowHint(GLFW_ACCUM_GREEN_BITS, config.accumGreenBits);
        glfwWindowHint(GLFW_ACCUM_BLUE_BITS, config.accumBlueBits);
        glfwWindowHint(GLFW_ACCUM_ALPHA_BITS, config.accumAlphaBits);



        //    int         auxBuffers;
        //GLFW_AUX_BUFFERS 	0 	0 to INT_MAX
        // case GLFW_AUX_BUFFERS: _glfw.hints.framebuffer.auxBuffers = value;
        //    GLFWbool    stereo;
        // case GLFW_STEREO: _glfw.hints.framebuffer.stereo = value ? GLFW_TRUE : GLFW_FALSE;
        glfwWindowHint(GLFW_SAMPLES, config.samples);
        //    GLFWbool    sRGB;
        // case GLFW_SRGB_CAPABLE: _glfw.hints.framebuffer.sRGB = value ? GLFW_TRUE : GLFW_FALSE;
        glfwWindowHint(GLFW_DOUBLEBUFFER, config.doubleBuffered);
        glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, config.transparent);




        // struct _GLFWctxconfig
        //{
        //    int           client;
        //glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
        //    int           source;
        //    int           major;
        //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
        //    int           minor;
        //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        //    GLFWbool      forward;
        //    GLFWbool      debug;
        //    GLFWbool      noerror;
        //    int           profile;
        //    int           robustness;
        //    int           release;
        //    _GLFWwindow*  share;
        //    struct {
        //        GLFWbool  offline;
        //    } nsgl;
        //};
        // case GLFW_CLIENT_API: _glfw.hints.context.client = value;
        // case GLFW_CONTEXT_CREATION_API: _glfw.hints.context.source = value;
        // case GLFW_CONTEXT_VERSION_MAJOR: _glfw.hints.context.major = value;
        // case GLFW_CONTEXT_VERSION_MINOR: _glfw.hints.context.minor = value;
        // case GLFW_CONTEXT_ROBUSTNESS: _glfw.hints.context.robustness = value;
        // case GLFW_OPENGL_FORWARD_COMPAT: _glfw.hints.context.forward = value ? GLFW_TRUE : GLFW_FALSE;
        // case GLFW_OPENGL_DEBUG_CONTEXT: _glfw.hints.context.debug = value ? GLFW_TRUE : GLFW_FALSE;
        // case GLFW_CONTEXT_NO_ERROR: _glfw.hints.context.noerror = value ? GLFW_TRUE : GLFW_FALSE;
        // case GLFW_OPENGL_PROFILE: _glfw.hints.context.profile = value;
        // case GLFW_CONTEXT_RELEASE_BEHAVIOR: _glfw.hints.context.release = value;

        // case GLFW_COCOA_RETINA_FRAMEBUFFER: _glfw.hints.window.ns.retina = value ? GLFW_TRUE : GLFW_FALSE;
        // case GLFW_COCOA_GRAPHICS_SWITCHING: _glfw.hints.context.nsgl.offline = value ? GLFW_TRUE : GLFW_FALSE;

        // case GLFW_REFRESH_RATE: _glfw.hints.refreshRate = value;
    }

    void Window::createWindow(Config &config) {
        this->glfwWindow = glfwCreateWindow(config.width, config.height, config.title.c_str(), nullptr, nullptr);
        if (this->glfwWindow == nullptr) throw std::runtime_error("Window::Window: Error constructing GLFW window.");
    }

    void Window::registerCallbacks(Config &config) {
        glfwSetWindowCloseCallback(this->glfwWindow, [](GLFWwindow *window) {
            auto *thisWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
            // reset flag and push event to properly terminate the window
            glfwSetWindowShouldClose(window, GLFW_FALSE);

            // close if window-callback returns true or doesn't exist
            if(thisWindow->windowCallback != nullptr) {
                if(thisWindow->windowCallback->close()) {
                    GLFWM::pushWindowEvent<DestroyWindow>(*thisWindow->getApplication().windowHandle);
                }
            } else GLFWM::pushWindowEvent<DestroyWindow>(*thisWindow->getApplication().windowHandle);
        });

        glfwSetFramebufferSizeCallback(glfwWindow, [](GLFWwindow *window, int width, int height) {
            auto *thisWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
            thisWindow->windowWidth = width;
            thisWindow->windowHeight = height;
            thisWindow->application->resize(width, height);
        });

        if (this->windowCallback != nullptr) {
            glfwSetWindowPosCallback(glfwWindow, [](GLFWwindow *window, int xpos, int ypos) {
                auto *thisWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
            });
            glfwSetWindowSizeCallback(glfwWindow, [](GLFWwindow *window, int width, int height) {
                auto *thisWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
            });
            glfwSetWindowRefreshCallback(glfwWindow, [](GLFWwindow *window) {
                auto *thisWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
            });
            glfwSetWindowFocusCallback(glfwWindow, [](GLFWwindow *window, int focused) {
                auto *thisWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
            });
            glfwSetWindowIconifyCallback(glfwWindow, [](GLFWwindow *window, int iconified) {
                auto *thisWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
            });
            glfwSetWindowMaximizeCallback(glfwWindow, [](GLFWwindow *window, int maximized) {
                auto *thisWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
            });
            glfwSetWindowContentScaleCallback(glfwWindow, [](GLFWwindow *window, float xscale, float yscale) {
                auto *thisWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
            });
        }

        // TODO: glfwSetDropCallback

        if (this->keyCallback != nullptr) {
            glfwSetKeyCallback(glfwWindow, [](GLFWwindow *window, int key, int scancode, int action, int mode) {
                auto *thisWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
                thisWindow->group.pushInternalWindowEvent(
                        new InternalKeyEvent(
                                *thisWindow->application->windowHandle,
                                key, scancode, action, mode)
                );
            });
            glfwSetCharCallback(glfwWindow, [](GLFWwindow *window, unsigned int codepoint) {
                auto *thisWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
                thisWindow->group.pushInternalWindowEvent(
                        new InternalCharEvent(*thisWindow->application->windowHandle, codepoint)
                );
            });
            glfwSetCharModsCallback(glfwWindow, [](GLFWwindow *window, unsigned int codepoint, int mods) {
                auto *thisWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
                thisWindow->group.pushInternalWindowEvent(
                        new InternalCharModsEvent(*thisWindow->application->windowHandle, codepoint, mods)
                );
            });
        }

        if (this->mouseCallback != nullptr) {
            glfwSetMouseButtonCallback(glfwWindow, [](GLFWwindow *window, int button, int action, int mods) {
                auto *thisWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
                thisWindow->group.pushInternalWindowEvent(
                        new InternalMouseButtonEvent(*thisWindow->application->windowHandle, button, action, mods)
                );
            });
            glfwSetCursorPosCallback(glfwWindow, [](GLFWwindow *window, double xpos, double ypos) {
                auto *thisWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
                thisWindow->group.pushInternalWindowEvent(
                        new InternalMousePosEvent(*thisWindow->application->windowHandle, xpos, ypos)
                );
            });
            glfwSetScrollCallback(glfwWindow, [](GLFWwindow *window, double xoffset, double yoffset) {
                auto *thisWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
                thisWindow->group.pushInternalWindowEvent(
                        new InternalMouseScrollEvent(*thisWindow->application->windowHandle, xoffset, yoffset)
                );
            });
            glfwSetCursorEnterCallback(glfwWindow, [](GLFWwindow *window, int entered) {
                auto *thisWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
                thisWindow->group.pushInternalWindowEvent(
                        new InternalMouseEnterEvent(*thisWindow->application->windowHandle, entered)
                );
            });
        }

        // TODO: if (this->joystickCallback != nullptr)
    }

    void Window::render() {
        for (auto &p: plugins) p->preRenderWindow();

        // Measure speed
        double currentTime = glfwGetTime();
        frameCount++;
        if (currentTime - previousTime >= 1.0) {
            //std::cout << frameCount << std::endl; // Display the frame count here any way you want.
            frameCount = 0;
            previousTime = currentTime;
        }

        application->render(1.0f, lastFrameTime); // TODO: properly calc dt

        for (auto &p: plugins) p->postRenderWindow();
    }

    void Window::scheduleTermination() {
        this->application->dispose();
        for (auto &p: this->plugins) p->dispose();

        std::unique_lock<std::mutex> lk(mutex);
        this->terminated = true;
        lk.unlock();
        cv.notify_one();
    }

    Window::~Window() {
        for (auto p: plugins) delete p;
    }

    Application &Window::getApplication() {
        return *application;
    }

    GLFWwindow *Window::getGlfwWindow() {
        return glfwWindow;
    }

    KeyCallback *Window::getKeyCallback() {
        return this->keyCallback;
    }

    MouseCallback *Window::getMouseCallback() {
        return this->mouseCallback;
    }

    bool Window::isTerminated() const {
        return terminated;
    }

} // namespace