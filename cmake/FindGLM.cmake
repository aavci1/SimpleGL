# Find the OpenGL Extension Wrangler Library (GLM)
# This module defines the following variables:
#  GLM_INCLUDE_DIRS - include directories for GLM
#  GLM_FOUND - true if GLM has been found and can be used

find_path(GLM_INCLUDE_DIR glm.hpp
  HINTS ENV GLM_DIR
  PATH_SUFFIXES include glm
)

set(GLM_INCLUDE_DIRS ${GLM_INCLUDE_DIR})

include(${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake)
find_package_handle_standard_args(GLM REQUIRED_VARS GLM_INCLUDE_DIR)

mark_as_advanced(GLM_INCLUDE_DIR)
