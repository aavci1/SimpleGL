project (SimpleGL)
# set cmake minimum cmake version
cmake_minimum_required (VERSION 2.8)
# extra definitions
add_definitions(-Wall -O2 -msse2 -std=c++11)
# add sources
set (SOURCES
  Animation.cpp
  AnimationTrack.cpp
  Bone.cpp
  Camera.cpp
  DirectionalLight.cpp
  InputStream.cpp
  Instance.cpp
  Light.cpp
  Material.cpp
  Mesh.cpp
  OutputStream.cpp
  PointLight.cpp
  Program.cpp
  Root.cpp
  SceneNode.cpp
  SceneObject.cpp
  SubMesh.cpp
  SpotLight.cpp
  Viewport.cpp
  Window.cpp
)
# add target
add_library (SimpleGL SHARED ${SOURCES})
# link libs
target_link_libraries (SimpleGL GL GLEW freeimage)
