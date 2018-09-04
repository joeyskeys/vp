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
    table.h
    render_obj.h
    )

set(SOURCES
    dynamic_mesh.cpp
    mesh.cpp
    light.cpp
    camera.cpp
    shader.cpp
    material.cpp
    utils.cpp
    table.cpp
    render_obj.cpp
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
