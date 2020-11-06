add_library(fmod STATIC dummy.c)
target_include_directories(fmod PUBLIC inc)

if (UNIX)
    get_filename_component(FMOD_LIB lib/linux/${CMAKE_SYSTEM_PROCESSOR}/libfmod.so.12 REALPATH)
    set(FMOD_OBJ ${FMOD_LIB})

    target_link_libraries(fmod PUBLIC ${FMOD_LIB})
endif()

if (WIN32)
    math(EXPR BITS "8*${CMAKE_SIZEOF_VOID_P}")

    set(CPU x86)
    if (${BITS} EQUAL 64)
        SET(CPU x86_64)
    endif()

    get_filename_component(FMOD_LIB lib/win/${CPU}/fmod_vc.lib REALPATH)
    get_filename_component(FMOD_OBJ lib/win/${CPU}/fmod.dll REALPATH)

    target_link_libraries(fmod PUBLIC ${FMOD_LIB})
endif()

add_custom_command (TARGET fmod POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${FMOD_OBJ} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
