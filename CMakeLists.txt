project (SimpleGL)
# set cmake minimum cmake version
cmake_minimum_required (VERSION 2.8)
# extra definitions
add_definitions(-O2 -msse2)
# add sources
set (SOURCES
  src/Main.cpp
  src/AbstractShader.cpp
  src/FragmentShader.cpp
  src/VertexShader.cpp
  src/ShaderProgram.cpp
  src/SubMesh.cpp
  src/Texture.cpp
)
# opengl target
add_executable (SimpleGL WIN32 ${SOURCES} resources.rc)
target_link_libraries (SimpleGL GL GLEW glfw freeimage)
