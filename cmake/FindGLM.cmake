# Find the OpenGL Mathematics Library (GLM)
# This module defines the following variables:
#  GLM_INCLUDE_DIRS - include directories for GLM
#  GLM_FOUND - true if GLM has been found and can be used

include(${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake)

find_path(GLM_INCLUDE_DIR glm/glm.hpp
    HINTS ENV GLM_DIR
    PATH_SUFFIXES include
)

set(GLM_INCLUDE_DIRS ${GLM_INCLUDE_DIR})

find_package_handle_standard_args(GLM REQUIRED_VARS GLM_INCLUDE_DIRS)

mark_as_advanced(GLM_INCLUDE_DIRS)
