CMAKE_MINIMUM_REQUIRED(VERSION 2.8.10)

PROJECT(SimpleGL_Editor)

ADD_DEFINITIONS(-Wall -std=c++11 -O2 -g)

# Qt
FIND_PACKAGE(Qt4 REQUIRED)
IF(QT_FOUND)
  SET(QT_USE_QTOPENGL TRUE)
  INCLUDE(${QT_USE_FILE})
  INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/editor)
ELSE()
  MESSAGE(FATAL_ERROR "Qt not found! Please make sure Qt is installed.")
ENDIF(QT_FOUND)

# Assimp
FIND_PACKAGE(Assimp REQUIRED)
IF(Assimp_FOUND)
  INCLUDE_DIRECTORIES(${Assimp_INCLUDE_DIR})
ELSE()
  MESSAGE(FATAL_ERROR "Assimp not found! Please make sure Assimp is installed and Assimp_DIR is set correctly.")
ENDIF(Assimp_FOUND)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/lib ${CMAKE_SOURCE_DIR}/editor/src)
LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/lib)

SET(SOURCES
  src/AssimpImporter.cpp
  src/GLWidget.cpp
  src/Main.cpp
  src/MainWindow.cpp
)

QT4_WRAP_UI(UI_SOURCES
  src/MainWindow.ui
)

QT4_WRAP_CPP(MOC_SOURCES
  src/GLWidget.h
  src/MainWindow.h
)

QT4_ADD_RESOURCES(RESOURCES
  resources.qrc
)

ADD_EXECUTABLE(SimpleGL-Editor WIN32 ${SOURCES} ${MOC_SOURCES} ${UI_SOURCES} ${RESOURCES} resources.rc)
TARGET_LINK_LIBRARIES(SimpleGL-Editor SimpleGL ${QT_LIBRARIES} ${ASSIMP_LIBRARIES})
