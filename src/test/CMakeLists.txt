set(SOURCES
    main.cpp)

add_executable(test1
    main.cpp
    )

target_link_libraries(test1
    general
        io
        model
        ${GLEW_LIBRARIES}
    )

source_group("Source Files"
    FILES
        $(SOURCES)
)
