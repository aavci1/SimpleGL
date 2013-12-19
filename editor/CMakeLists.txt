cmake_minimum_required(VERSION 2.8.11)

project(SimpleGL_Editor)

add_definitions(-Wall -std=c++11 -O2 -g)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Qt5
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)

# Eliminate a warning when building in Windows that relates
# to static linking of Qt executables to qtmain.lib.
# This policy was introduced in CMake version 2.8.11.
# CMake version 2.8.11.2 warns when the policy is not set
# and uses OLD behavior.
if(POLICY CMP0020)
    cmake_policy(SET CMP0020 NEW)
endif()

# Assimp
find_package(Assimp REQUIRED)
if(ASSIMP_FOUND)
  include_directories(${ASSIMP_INCLUDE_DIR})
else()
  message(FATAL_ERROR "Assimp not found! Please make sure Assimp is installed and ASSIMP_DIR is set correctly.")
endif(ASSIMP_FOUND)

include_directories(${CMAKE_SOURCE_DIR}/lib ${CMAKE_SOURCE_DIR}/editor/src)
link_directories(${CMAKE_BINARY_DIR}/lib)

set(SOURCES
  src/AssimpImporter.cpp
  src/GLWidget.cpp
  src/Main.cpp
  src/MainWindow.cpp
)

qt5_wrap_ui(UI_SOURCES
  src/MainWindow.ui
)

qt5_add_resources(RESOURCES
  resources.qrc
)

add_executable(SimpleGL-Editor WIN32 ${SOURCES} ${MOC_SOURCES} ${UI_SOURCES} ${RESOURCES} resources.rc)
target_link_libraries(SimpleGL-Editor SimpleGL ${ASSIMP_LIBRARIES})
qt5_use_modules(SimpleGL-Editor Widgets)
