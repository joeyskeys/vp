if(NOT GLM_ROOT)
    if(WIN32)
        set(GLM_ROOT "G:/WorkSpace/installed_libs/glm")
    else()
        set(GLM_ROOT "/usr/local")
    endif()
endif()

find_path(GLM_INCLUDE_DIRS
    NAMES glm/glm.hpp
    PATHS ${GLM_ROOT}/include
    )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLM
    REQUIRED_VARS GLM_INCLUDE_PATH)
