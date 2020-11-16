add_library(IceSDKBindings STATIC Lib.cc)
target_link_libraries(IceSDKBindings PUBLIC IceSDKCore IceSDKScripting)
