set(HEADERS
	cache.h
	block.h
	manager.h
	mesh.h
	light.h
	camera.h
	material.h
	)

set(SOURCES
	mesh.cpp
	light.cpp
	camera.cpp
	material.cpp
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
