add_library(viewport STATIC
	viewport.h
    viewport.cpp
	)

if (WIN32)
	set(PLATFORM_SPEC_LIBS
		opengl32.lib)
else()
	set(PLATFORM_SPEC_LIBS
        ${OPENGL_LIBRARIES}
		)
endif()

target_link_libraries(viewport
	general
        Qt5::Widgets
        Qt5::Core
        Qt5::Gui
        ${GLEW_LIBRARIES}
        ${PLATFORM_SPEC_LIBS}
	)

source_group("Header Files"
	FILES
		viewport.h
)

source_group("Source Files"
	FILES
		viewport.cpp
)
