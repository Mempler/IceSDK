add_library(bgfx STATIC src/amalgamated.cpp)

target_link_libraries(bgfx PUBLIC bimg bx)
target_include_directories(bgfx PUBLIC include PRIVATE 3rdparty 3rdparty/cgltf 3rdparty/dxsdk/include 3rdparty/khronos)

if (UNIX)
    target_link_libraries(bgfx PUBLIC GL X11)
endif()

if (WIN32)
    target_link_libraries(bgfx PUBLIC psapi)
endif()
