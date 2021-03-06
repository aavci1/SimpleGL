# Find FreeImage
# This module defines the following variables:
#  FREEIMAGE_INCLUDE_DIRS - include directories for FreeImage
#  FREEIMAGE_LIBRARIES - libraries to link against FreeImage
#  FREEIMAGE_FOUND - true if FreeImage has been found and can be used

include(${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake)

find_path(FREEIMAGE_INCLUDE_DIR FreeImage.h
    HINTS ENV FREEIMAGE_DIR
    PATH_SUFFIXES include
)

find_library(FREEIMAGE_LIBRARY NAMES FreeImage freeimage
    HINTS ENV FREEIMAGE_DIR 
    PATH_SUFFIXES lib lib64
)

set(FREEIMAGE_INCLUDE_DIRS ${FREEIMAGE_INCLUDE_DIR})
set(FREEIMAGE_LIBRARIES ${FREEIMAGE_LIBRARY})

find_package_handle_standard_args(FREEIMAGE REQUIRED_VARS FREEIMAGE_LIBRARIES FREEIMAGE_INCLUDE_DIRS)

mark_as_advanced(FREEIMAGE_LIBRARIES FREEIMAGE_INCLUDE_DIRS)
 
