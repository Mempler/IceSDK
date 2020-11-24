cmake_minimum_required(VERSION 3.16)
project(IceSDK)

set(CMAKE_CXX_STANDARD 17)

math(EXPR BITS "8*${CMAKE_SIZEOF_VOID_P}")

# Fix for FMOD
if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
    set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

# Easier access
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(ICESDK_USE_SDL2     "Use SDL2"               ON)
option(ICESDK_USE_FMOD     "Use FMOD"               ON)
option(ICESDK_USE_PROFILER "Use IceSDK's Profiler" OFF)

option(ICESDK_BUILD_TESTS "Build Test cases" OFF)

# Asset Compiler Function
function(target_link_assets)
    set(flags OPTIONAL FLAGS)
    set(oneValueArgs TARGET)
    set(multiValueArgs SOURCES)

    include(CMakeParseArguments)
    cmake_parse_arguments(ASSET
            "${flags}"
            "${oneValueArgs}"
            "${multiValueArgs}"
            ${ARGN})

    # message(${ASSET_TARGET})
    # message(${ASSET_SOURCES})

    # TODO: actually use ASSET_SOURCES
    # TODO: DO NOT hardcode Assets/ in python file

    find_package (Python REQUIRED)

    message(${CMAKE_CURRENT_SOURCE_DIR})
    add_custom_command(
            TARGET ${ASSET_TARGET}
            COMMAND
                ${Python_EXECUTABLE}
                ${IceSDK_SOURCE_DIR}/tools/compile_assets.py
                    --target ${CMAKE_CURRENT_SOURCE_DIR}
                    --out ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    )
endfunction()


add_subdirectory(third-party)
add_subdirectory(IceSDK)
add_subdirectory(examples)

if (ICESDK_BUILD_TESTS)
    add_subdirectory(Tests)
endif()

add_subdirectory(tools/sources/AssetCompiler)
