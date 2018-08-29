set(HEADERS
    cache.h
    block.h
    manager.h
    mesh.h
    dynamic_mesh.h
    light.h
    camera.h
    shader.h
    material.h
    utils.h
    uniform_table.h
    )

set(SOURCES
    dynamic_mesh.cpp
    mesh.cpp
    light.cpp
    camera.cpp
    shader.cpp
    material.cpp
    utils.cpp
    uniform_table.cpp
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
