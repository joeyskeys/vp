add_library(viewport STATIC
    viewport.cpp
)

target_link_libraries(viewport
    optimized
        Qt5::Widgets
        Qt5::Core
        Qt5::Gui
)
