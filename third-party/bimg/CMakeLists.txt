add_library(bimg STATIC
        src/image.cpp
        src/image_cubemap_filter.cpp
        src/image_decode.cpp
        src/image_encode.cpp
        src/image_gnf.cpp)

add_subdirectory(3rdparty/iqa)
add_subdirectory(3rdparty/astc-codec)

target_include_directories(bimg PUBLIC include 3rdparty)
target_link_libraries(bimg PUBLIC bx iqa astc-codec)

if (ANDROID)
    add_subdirectory(3rdparty/astc)
    add_subdirectory(3rdparty/edtaa3)
    add_subdirectory(3rdparty/etc1)
    add_subdirectory(3rdparty/etc2)

    target_link_libraries(bimg PUBLIC astc edtaa3 etc1 etc2)
endif()
