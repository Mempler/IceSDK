add_library(IceSDKScripting STATIC
    Core.cc
    Native.c
)
target_precompile_headers(IceSDKScripting PUBLIC pch.h)
