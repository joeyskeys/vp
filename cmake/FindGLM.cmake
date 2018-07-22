if(NOT GLM_ROOT)
    if(WIN32)
        set(GLM_ROOT "G:/WorkSpace/installed_libs/glm" CACHE PATH "Location of glm")
    else()
        set(GLM_ROOT "/usr/local" CACHE PATH "Location of glm")
    endif()
endif()

find_path(GLM_INCLUDE_DIRS
    NAMES glm/glm.hpp
    PATHS ${GLM_ROOT}/include
          /usr/include
    )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLM
    REQUIRED_VARS GLM_INCLUDE_PATH)
