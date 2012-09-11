project (SimpleGL-Editor)
# set cmake minimum cmake version
cmake_minimum_required (VERSION 2.8)
# find qt
find_package (Qt4 REQUIRED)
# use qt opengl module
set (QT_USE_QTOPENGL TRUE)
# include qt use file
include (${QT_USE_FILE})
# include additional directories  needed for moc files etc.
include_directories (../lib ${CMAKE_SOURCE_DIR}/editor/src ${CMAKE_BINARY_DIR} ${CMAKE_BINARY_DIR}/editor)
# set lib directories to contain build_directory/lib
link_directories (${CMAKE_BINARY_DIR}/lib)
# add sources
set (SOURCES
  src/AssimpImporter.cpp
  src/FbxImporter.cpp
  src/GLWidget.cpp
  src/Main.cpp
  src/MainWindow.cpp
)
# add ui files
qt4_wrap_ui (UI_SOURCES
  src/MainWindow.ui
)
# add headers
qt4_wrap_cpp (MOC_SOURCES
  src/GLWidget.h
  src/MainWindow.h
)
# add resources
qt4_add_resources (RESOURCES
  resources.qrc
)
# opengl target
add_executable (SimpleGL-Editor WIN32 ${SOURCES} ${MOC_SOURCES} ${UI_SOURCES} ${RESOURCES} resources.rc)
target_link_libraries (SimpleGL-Editor SimpleGL ${QT_LIBRARIES} assimp "/usr/lib/gcc4/x64/libfbxsdk-2013.2.so")
