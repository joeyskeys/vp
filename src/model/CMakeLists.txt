set(HEADERS
    cache.h
    mesh.h
    camera.h
    )

set(SOURCES
    mesh.cpp
    camera.cpp
    )

add_library(model STATIC
    ${HEADERS}
    ${SOURCES}
    )

source_group("Header Files"
	FILES
        ${HEADERS}
    )

source_group("Source Files"
	FILES
        ${SOURCES}
    )
