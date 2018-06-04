add_library(viewport STATIC
	viewport.h
    viewport.cpp
	)

target_link_libraries(viewport
	general
        Qt5::Widgets
        Qt5::Core
        Qt5::Gui
	)

source_group("Header Files"
	FILES
		viewport.h
)

source_group("Source Files"
	FILES
		viewport.cpp
)