project (SimpleGL)
# set cmake minimum cmake version
cmake_minimum_required (VERSION 2.8)
# extra definitions
add_definitions(-O2 -msse2)
# add sources
set (SOURCES
  src/Camera.cpp
  src/CubeMesh.cpp
  src/DeferredRenderer.cpp
  src/FragmentShader.cpp
  src/GBuffer.cpp
  src/Main.cpp
  src/Mesh.cpp
  src/Node.cpp
  src/Renderer.cpp
  src/Shader.cpp
  src/ShaderProgram.cpp
  src/Texture.cpp
  src/VertexShader.cpp
)
# opengl target
add_executable (SimpleGL WIN32 ${SOURCES} resources.rc)
target_link_libraries (SimpleGL GL GLEW glfw freeimage)
