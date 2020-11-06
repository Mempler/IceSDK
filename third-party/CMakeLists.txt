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

    add_subdirectory(sdl2)
endif()

# Graphics Wrapper
add_subdirectory(bgfx)
add_subdirectory(bimg)
add_subdirectory(bx)

# Font rendering
add_subdirectory(freetype)
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
