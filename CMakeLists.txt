project (SimpleGL)
# set cmake minimum cmake version
cmake_minimum_required (VERSION 2.8)
# find qt
find_package (Qt4 REQUIRED)
# use qt opengl module
set (QT_USE_QTOPENGL TRUE)
# include qt use file
include (${QT_USE_FILE})
# include additional directories -- needed for moc files etc.
include_directories (${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/test)
# extra definitions
add_definitions(-O2 -msse2 -Wall -std=c++11)
# add sources
set (SOURCES
  src/AssimpImporter.cpp
  src/Bone.cpp
  src/Camera.cpp
  src/DirectionalLight.cpp
  src/Instance.cpp
  src/Light.cpp
  src/Material.cpp
  src/Mesh.cpp
  src/PointLight.cpp
  src/Program.cpp
  src/Renderable.cpp
  src/Root.cpp
  src/SceneNode.cpp
  src/SceneObject.cpp
  src/SubMesh.cpp
  src/SpotLight.cpp
  src/Texture.cpp
  src/Viewport.cpp
  src/Window.cpp
  test/Main.cpp
  test/MainWindow.cpp
  test/QSGLWidget.cpp
)
# add ui files
qt4_wrap_ui (UI_SOURCES
  test/MainWindow.ui
)
# add headers
qt4_wrap_cpp (MOC_SOURCES
  test/MainWindow.h
  test/QSGLWidget.h
)
# add resources
qt4_add_resources (RESOURCES
  resources.qrc
)
# opengl target
add_executable (SimpleGL WIN32 ${SOURCES} ${MOC_SOURCES} ${UI_SOURCES} ${RESOURCES} resources.rc)
target_link_libraries (SimpleGL ${QT_LIBRARIES} GL GLEW freeimage assimp)
