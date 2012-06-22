project (SimpleGL)
# set cmake minimum cmake version
cmake_minimum_required (VERSION 2.8)
# extra definitions
add_definitions(-O2 -msse2 -Wall -std=c++11)
# add sources
set (SOURCES
    src/Camera.cpp
    src/DirectionalLight.cpp
    src/FrameBuffer.cpp
    src/Instance.cpp
    src/Light.cpp
    src/Main.cpp
    src/Material.cpp
    src/Mesh.cpp
    src/PointLight.cpp
    src/Program.cpp
    src/Renderable.cpp
    src/Root.cpp
    src/SceneNode.cpp
    src/SceneObject.cpp
    src/SpotLight.cpp
    src/Texture.cpp
    src/Viewport.cpp
    src/Window.cpp
)
# opengl target
add_executable (SimpleGL WIN32 ${SOURCES} resources.rc)
target_link_libraries (SimpleGL GL GLEW glfw freeimage assimp)
