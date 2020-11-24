add_library(imgui STATIC
        imgui.cpp
        imgui_demo.cpp
        imgui_draw.cpp
        imgui_widgets.cpp)

if (ICESDK_USE_SDL2)
    target_sources(imgui PUBLIC imgui_impl_sdl.cpp)
endif()

target_include_directories(imgui PUBLIC . ../sdl2/include)
