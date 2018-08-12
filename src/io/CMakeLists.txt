set(HEADERS
	obj_loader.h
	)

set(SOURCES
	obj_loader.cpp)

add_library(io STATIC
	${HEADERS}
	${SOURCES}
	)

target_link_libraries(io
    general
        model
    )

source_group("Header Files"
	FILES
		${HEADERS}
)

source_group("Source Files"
	FILES
		${SOURCES}
)
