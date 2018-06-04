cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

if (NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build, options are: ${CMAKE_CONFIGURATION_TYPES}")
endif()

project(vp)

set(CMAKE_AUTOMOC ON)

set(CMAKE_PREFIX_PATH "G:/QT/program/5.10.1/msvc2013_64" CACHE PATH "Location of Qt")
find_package(Qt5Widgets)
find_package(Qt5Core)
find_package(Qt5Gui)

include_directories(${Qt5Widgets_INCLUDE_DIRS})
include_directories(${Qt5Core_INCLUDE_DIRS})
include_directories(${Qt5Gui_INCLUDE_DIRS})

add_subdirectory(src)
