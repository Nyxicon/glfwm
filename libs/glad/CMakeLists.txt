add_library(glad STATIC
        include/KHR/khrplatform.h
        include/glad/glad.h
        src/glad.c)

target_include_directories(glad
        PUBLIC
        # where external projects will look for the library's public headers
        $<INSTALL_INTERFACE:include>
        # where top-level project will look for the library's public headers
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE
        # where the library itself will look for its internal headers
        ${CMAKE_CURRENT_SOURCE_DIR}
        )