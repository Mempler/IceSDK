cmake_minimum_required(VERSION 3.17)
project(IceSDK)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(ICESDK_USE_SDL2 "Use SDL2" ON)
# option(ICESDK_USE_FMOD "Use FMOD" ON)

# Fix for FMOD
if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
    set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

add_subdirectory(third-party)

set_target_properties(astc_inspector_cli PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)

add_subdirectory(IceSDK)

#if (ICESDK_USE_FMOD)
#    target_link_libraries(IceSDK PUBLIC fmod)
#    target_compile_definitions(IceSDK PUBLIC -DICESDK_FMOD=1)
#endif()

#add_executable(Game IceSDK/main.cc)
#target_link_libraries(Game PUBLIC IceSDK)
