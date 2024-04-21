add_library(imgui-1.90.5
        # Main Imgui files
        imgui.cpp
        imgui.h
        imconfig.h
        imgui_demo.cpp
        imgui_draw.cpp
        imgui_internal.h
        imgui_tables.cpp
        imgui_widgets.cpp
        imstb_rectpack.h
        imstb_textedit.h
        imstb_truetype.h
        # GLFW+OpenGL-specific files
        imgui_impl_glfw.cpp
        imgui_impl_glfw.h
        imgui_impl_opengl3_loader.h
        imgui_impl_opengl3.cpp
        imgui_impl_opengl3.h)

target_include_directories(imgui-1.90.5
        PUBLIC
        $<INSTALL_INTERFACE:/>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR})