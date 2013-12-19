# Find the OpenGL Extension Wrangler Library (GLEW)
# This module defines the following variables:
#  GLEW_INCLUDE_DIRS - include directories for GLEW
#  GLEW_LIBRARIES - libraries to link against GLEW
#  GLEW_FOUND - true if GLEW has been found and can be used

include(${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake)

find_path(GLEW_INCLUDE_DIR GL/glew.h 
    HINTS ENV GLEW_DIR
    PATH_SUFFIXES include
)

find_library(GLEW_LIBRARY NAMES GLEW glew32 glew glew32s 
    HINTS ENV GLEW_DIR
    PATH_SUFFIXES lib lib64
)

set(GLEW_INCLUDE_DIRS ${GLEW_INCLUDE_DIR})
set(GLEW_LIBRARIES ${GLEW_LIBRARY})

find_package_handle_standard_args(GLEW REQUIRED_VARS GLEW_LIBRARIES GLEW_INCLUDE_DIRS)

mark_as_advanced(GLEW_LIBRARIES GLEW_INCLUDE_DIRS)
