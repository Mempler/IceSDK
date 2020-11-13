#######################################################
#                      Audio                          #
#######################################################
if (ICESDK_USE_FMOD)
    add_subdirectory(fmod)
endif()

#######################################################
#                     Graphics                        #
#######################################################

# Windowing
if (ICESDK_USE_SDL2)
    # Fix for 32 bit
    if (MSVC)
        if (BITS EQUAL "32")
            set(OPT_DEF_LIBC ON)
        endif()
    endif()

    if (UNIX)
        find_package(SDL2 REQUIRED)

        # It's not static in reality
        # it's just a wrapper
        add_library(SDL2-static INTERFACE)
        target_link_libraries(SDL2-static INTERFACE ${SDL2_LIBRARIES})
        target_include_directories(SDL2-static INTERFACE ${SDL2_INCLUDE_DIRS})
    else()
        add_subdirectory(sdl2)
    endif()
endif()

# Graphics Wrapper
add_subdirectory(bgfx)
add_subdirectory(bimg)
add_subdirectory(bx)

# Font rendering

if (UNIX)
    # Find freetype
    find_package(Freetype REQUIRED)
    add_library(freetype INTERFACE)
    target_link_libraries(freetype INTERFACE ${FREETYPE_LIBRARIES})
    target_include_directories(freetype INTERFACE ${FREETYPE_INCLUDE_DIRS})
else()
    add_subdirectory(freetype)
endif()
add_subdirectory(harfbuzz)

#######################################################
#                       Math                          #
#######################################################
add_subdirectory(glm)

#######################################################
#                      Utils                          #
#######################################################
add_subdirectory(entt)
add_subdirectory(imgui)
add_subdirectory(spdlog)

#######################################################
#                Tests/Benchmarks                     #
#######################################################
#add_subdirectory(gbenchmark)

if(ICESDK_BUILD_TESTS)
    add_subdirectory(googletest)
endif()
