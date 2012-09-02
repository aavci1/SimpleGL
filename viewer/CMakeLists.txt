project (SimpleGLTest)
# set cmake minimum cmake version
cmake_minimum_required (VERSION 2.8)
# find qt
find_package (Qt4 REQUIRED)
# use qt opengl module
set (QT_USE_QTOPENGL TRUE)
# include qt use file
include (${QT_USE_FILE})
# include additional directories  needed for moc files etc.
include_directories (../lib ${CMAKE_SOURCE_DIR}/viewer ${CMAKE_BINARY_DIR} ${CMAKE_BINARY_DIR}/viewer)
# set lib directories to contain build_directory/lib
link_directories (${CMAKE_BINARY_DIR}/lib)
# extra definitions
add_definitions(-O2 -msse2 -Wall -std=c++11)
# add sources
set (SOURCES
  AssimpImporter.cpp
  Main.cpp
  MainWindow.cpp
  QSGLWidget.cpp
)
# add ui files
qt4_wrap_ui (UI_SOURCES
  MainWindow.ui
)
# add headers
qt4_wrap_cpp (MOC_SOURCES
  MainWindow.h
  QSGLWidget.h
)
# add resources
qt4_add_resources (RESOURCES
  resources.qrc
)
# opengl target
add_executable (SimpleGLTest WIN32 ${SOURCES} ${MOC_SOURCES} ${UI_SOURCES} ${RESOURCES} resources.rc)
target_link_libraries (SimpleGLTest SimpleGL ${QT_LIBRARIES} assimp)