add_library(bx STATIC src/amalgamated.cpp)
target_include_directories(bx PUBLIC include 3rdparty)

if(MSVC)
    target_include_directories(bx PUBLIC include/compat/msvc)
endif()

if(MSYS OR MINGW)
    target_include_directories(bx PUBLIC include/compat/mingw)
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    target_include_directories(bx PUBLIC include/compat/osx)
endif()

if(UNIX)
    target_link_libraries(bx dl)
endif()
