# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

#.rst:
# FindGLEW
# --------
#
# Find the OpenGL Extension Wrangler Library (GLEW)
#
# IMPORTED Targets
# ^^^^^^^^^^^^^^^^
#
# This module defines the :prop_tgt:`IMPORTED` target ``GLEW::GLEW``,
# if GLEW has been found.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# This module defines the following variables:
#
# ::
#
#   GLEW_INCLUDE_DIRS - include directories for GLEW
#   GLEW_LIBRARIES - libraries to link against GLEW
#   GLEW_FOUND - true if GLEW has been found and can be used

if(NOT GLEW_ROOT)
  if(WIN32)
    set(GLEW_ROOT "C:/Program Files/glew" CACHE PATH "Location of glew")
  else()
    set(GLEW_ROOT "/usr/local" CACHE PATH "Location of glew")
  endif()
endif()

find_path(GLEW_INCLUDE_DIR GL/glew.h
  PATHS
    ${GLEW_ROOT}/include
    /usr/include
  )

if(NOT GLEW_LIBRARY)
  find_library(GLEW_LIBRARY_RELEASE 
    NAMES
      GLEW glew32 glew glew32s 
    PATHS
      ${GLEW_ROOT}/lib/Release/x64
      /usr/lib
    )
  find_library(GLEW_LIBRARY_DEBUG NAMES GLEWd glew32d glewd PATH_SUFFIXES lib64)
endif ()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLEW
                                  REQUIRED_VARS GLEW_INCLUDE_DIR GLEW_LIBRARY_RELEASE)

if(GLEW_FOUND)
  set(GLEW_INCLUDE_DIRS ${GLEW_INCLUDE_DIR})

  if(NOT GLEW_LIBRARIES)
    set(GLEW_LIBRARIES ${GLEW_LIBRARY_RELEASE})
  endif()

  if (NOT TARGET GLEW::GLEW)
    add_library(GLEW::GLEW UNKNOWN IMPORTED)
    set_target_properties(GLEW::GLEW PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES "${GLEW_INCLUDE_DIRS}")

    if(GLEW_LIBRARY_RELEASE)
      set_property(TARGET GLEW::GLEW APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
      set_target_properties(GLEW::GLEW PROPERTIES IMPORTED_LOCATION_RELEASE "${GLEW_LIBRARY_RELEASE}")
    endif()

    if(GLEW_LIBRARY_DEBUG)
      set_property(TARGET GLEW::GLEW APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
      set_target_properties(GLEW::GLEW PROPERTIES IMPORTED_LOCATION_DEBUG "${GLEW_LIBRARY_DEBUG}")
    endif()

    if(NOT GLEW_LIBRARY_RELEASE AND NOT GLEW_LIBRARY_DEBUG)
      set_property(TARGET GLEW::GLEW APPEND PROPERTY IMPORTED_LOCATION "${GLEW_LIBRARY}")
    endif()
  endif()
endif()

mark_as_advanced(GLEW_INCLUDE_DIR)
