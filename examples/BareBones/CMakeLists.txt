add_executable(BareBones main.cc)

target_link_libraries(BareBones IceSDK)
target_link_assets(
        TARGET
            BareBones
        SOURCES
            Assets/Box.png
            Assets/Ground.png)
