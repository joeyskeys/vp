add_library(model STATIC
	cache.h
	mesh.h
    mesh.cpp
    )

source_group("Header Files"
	FILES
		cache.h
		mesh.h
)

source_group("Source Files"
	FILES
		mesh.cpp
)