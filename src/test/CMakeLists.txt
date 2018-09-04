set(SOURCES
    main.cpp)

add_executable(test1
    main.cpp
    )

if (WIN32)
    set(PLATFORM_SPEC_LIBS
        opengl32.lib)
else()
    set(PLATFORM_SPEC_LIBS
        ${OPENGL_LIBRARIES}
        )
endif()

target_link_libraries(test1
    general
        io
        model
        ${GLEW_LIBRARIES}
        ${PLATFORM_SPEC_LIBS}
    )

source_group("Source Files"
    FILES
        $(SOURCES)
)
