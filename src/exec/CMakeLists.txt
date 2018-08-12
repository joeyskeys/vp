add_executable(mainwin
    main.cpp
	)

target_link_libraries(mainwin
	general
		viewport
        model
        io
	    Qt5::Widgets
	    Qt5::Core
	    Qt5::Gui
	)

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	install (FILES
	${CMAKE_PREFIX_PATH}/bin/Qt5Widgetsd.dll
	${CMAKE_PREFIX_PATH}/bin/Qt5Guid.dll
	${CMAKE_PREFIX_PATH}/bin/Qt5Cored.dll
	DESTINATION
	${CMAKE_CURRENT_BINARY_DIR})
else()
	install (FILES
	${CMAKE_PREFIX_PATH}/bin/Qt5Widgets.dll
	${CMAKE_PREFIX_PATH}/bin/Qt5Gui.dll
	${CMAKE_PREFIX_PATH}/bin/Qt5Core.dll
	DESTINATION
	${CMAKE_CURRENT_BINARY_DIR})
endif()

source_group("Source Files"
	FILES
		main.cpp
)
