project (SimpleGL)
# set cmake minimum cmake version
cmake_minimum_required (VERSION 2.8)
# extra definitions
add_definitions(-O2 -msse2)
# add sources
set (SOURCES
  src/Camera.cpp
  src/Cube.cpp
  src/DirectionalLight.cpp
  src/FragmentShader.cpp
  src/GBuffer.cpp
  src/Light.cpp
  src/Main.cpp
  src/Material.cpp
  src/MaterialManager.cpp
  src/Mesh.cpp
  src/Node.cpp
  src/Plane.cpp
  src/PointLight.cpp
  src/Program.cpp
  src/Quad.cpp
  src/Renderer.cpp
  src/Shader.cpp
  src/Sphere.cpp
  src/Texture.cpp
  src/Util.cpp
  src/VertexShader.cpp
)
# opengl target
add_executable (SimpleGL WIN32 ${SOURCES} resources.rc)
target_link_libraries (SimpleGL GL GLEW glfw freeimage)
