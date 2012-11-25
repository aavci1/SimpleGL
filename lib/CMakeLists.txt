CMAKE_MINIMUM_REQUIRED(VERSION 2.8.10)

PROJECT(SimpleGL)

set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)

ADD_DEFINITIONS(-Wall -std=c++11 -O2 -g)

# OpenGL
FIND_PACKAGE(OpenGL REQUIRED)
IF(OPENGL_FOUND)
  INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})
ELSE()
  MESSAGE(FATAL_ERROR "OpenGL not found! Please make sure OpenGL is installed.")
ENDIF(OPENGL_FOUND)

# GLEW
FIND_PACKAGE(GLEW REQUIRED)
IF(GLEW_FOUND)
  INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIR})
ELSE()
  MESSAGE(FATAL_ERROR "GLEW not found! Please make sure GLEW is installed and GLEW_DIR is set correctly")
ENDIF(GLEW_FOUND)

# GLM
FIND_PACKAGE(GLM REQUIRED)
IF(GLM_FOUND)
  INCLUDE_DIRECTORIES(${GLM_INCLUDE_DIR})
ELSE()
  MESSAGE(FATAL_ERROR "GLM not found! Please make sure GLM is installed and GLM_DIR is set correctly")
ENDIF(GLM_FOUND)

# FreeImage
FIND_PACKAGE(FreeImage REQUIRED)
IF(FREEIMAGE_FOUND)
  INCLUDE_DIRECTORIES(${FREEIMAGE_INCLUDE_DIR})
ELSE()
  MESSAGE(FATAL_ERROR "FreeImage not found! Please make sure FreeImage is installed and FREEIMAGE_DIR is set correctly.")
ENDIF(FREEIMAGE_FOUND)

SET(SOURCES
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
  Model.cpp
  OutputStream.cpp
  PointLight.cpp
  Program.cpp
  Root.cpp
  SceneNode.cpp
  SceneObject.cpp
  SpotLight.cpp
  Viewport.cpp
  Window.cpp
)

ADD_LIBRARY(SimpleGL SHARED ${SOURCES})
TARGET_LINK_LIBRARIES(SimpleGL ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${FREEIMAGE_LIBRARIES})
