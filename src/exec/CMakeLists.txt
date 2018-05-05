add_executable(mainwin
    main.cpp
)

target_link_libraries(mainwin
    optimized
        viewport
        Qt5::Widgets
        Qt5::Core
        Qt5::Gui
)
