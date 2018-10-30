cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

if (NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build, options are: ${CMAKE_CONFIGURATION_TYPES}")
endif()

set(CMAKE_CXX_STANDARD 11)
cmake_policy(SET CMP0074 NEW)

project(vp)

set(CMAKE_AUTOMOC ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

if (WIN32)
set(CMAKE_PREFIX_PATH "G:/QT/program/5.10.1/msvc2013_64" CACHE PATH "Location of Qt")
elseif(APPLE)
set(CMAKE_PREFIX_PATH "/usr/local/Cellar/qt/5.11.2")
endif()

find_package(Qt5Widgets)
find_package(Qt5Core)
find_package(Qt5Gui)

include_directories(${Qt5Widgets_INCLUDE_DIRS}
                    ${Qt5Core_INCLUDE_DIRS}
                    ${Qt5Gui_INCLUDE_DIRS})

if (Qt5_POSITION_INDEPENDENT_CODE)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

find_package(GLM)
include_directories(${GLM_INCLUDE_DIRS})

find_package(GLEW)
include_directories(${GLEW_INCLUDE_DIRS})

if (UNIX)
    set(OpenGL_GL_PREFERENCE LEGACY)
    find_package(OpenGL)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src)
