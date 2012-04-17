#include "Camera.h"
#include "Cube.h"
#include "DirectionalLight.h"
#include "Material.h"
#include "MaterialManager.h"
#include "Node.h"
#include "Plane.h"
#include "PointLight.h"
#include "Renderer.h"
#include "Sphere.h"

#include <glm/glm.hpp>

#include <GL/glew.h>
#include <GL/glfw.h>

using namespace SimpleGL;

Camera *camera;
Renderer *renderer;

void GLFWCALL resize(int width, int height) {
  //   printf("new size: %dx%d\n", width, height);
  glViewport(0, 0, (GLsizei) width, (GLsizei) height);
  // update renderer's buffers' size
  if (renderer)
    renderer->setSize(width, height);
  // update perspective
  if (camera)
    camera->setAspectRatio(float(width) / float(height));
}

int main(int argc, char **argv) {
  if (!glfwInit()) {
    printf("error: can not initialize glfw.\n");
    return 1;
  }
  // request opengl version
  glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
  glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
  // request opengl profile
  glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // try to open the window
  if (!glfwOpenWindow(1024, 768, 8, 8, 8, 0, 24, 0, GLFW_WINDOW)) {
    printf("error: cannot open glfw window.\n");
    glfwTerminate();
    return 1;
  }
  // set window title
  glfwSetWindowTitle("SimpleGL");
  // set window resize call back, must be set after window creation
  glfwSetWindowSizeCallback(resize);
  // initialize glew, must be after window creation
  if (glewInit() != GLEW_OK) {
    printf("error: can not initialize glew.\n");
    return 1;
  }
  // get window width and height
  int width = 0, height = 0;
  glfwGetWindowSize(&width, &height);
  // create renderer
  // WARN: if renderer is initialized further down, some strange artifacts occur
  renderer = new Renderer(width, height);
  // create root node
  Node *rootNode = new Node();
  // create laminate material
  Material *laminateMaterial = MaterialManager::instance()->createMaterial("Laminate");
  laminateMaterial->setProgram("media/deferred_geometry_vp.glsl", "media/deferred_geometry_fp.glsl");
  laminateMaterial->addTexture("media/laminate.jpg");
  // create ceramics material
  Material *ebonyMaterial = MaterialManager::instance()->createMaterial("Ebony");
  ebonyMaterial->setProgram("media/deferred_geometry_vp.glsl", "media/deferred_geometry_fp.glsl");
  ebonyMaterial->addTexture("media/ebony.jpg");
  // create ceiling material
  Material *ceilingMaterial = MaterialManager::instance()->createMaterial("Ceiling");
  ceilingMaterial->setProgram("media/deferred_geometry_vp.glsl", "media/deferred_geometry_fp.glsl");
  ceilingMaterial->addTexture("media/ceiling.jpg");
  // create floor
  Plane *floor = new Plane(glm::vec2(1000, 1000), glm::vec2(10, 10));
  floor->setMaterialName("Laminate");
  Node *floorNode = rootNode->createChildNode();
  floorNode->attachMesh(floor);
  // create ceiling
  Plane *ceiling = new Plane(glm::vec2(1000, 1000), glm::vec2(10, 10));
  ceiling->setMaterialName("Ceiling");
  Node *ceilingNode = rootNode->createChildNode();
  ceilingNode->attachMesh(ceiling);
  ceilingNode->setPosition(0.0f, 200.0f, 0.0f);
  ceilingNode->roll(180);
  // create a cube
  Cube *cube = new Cube(glm::vec3(50.0f));
  cube->setMaterialName("Ebony");
  Node *cubeNode = new Node();
  cubeNode->setPosition(0.0f, 100.0f, 0.0f);
  cubeNode->attachMesh(cube);
  rootNode->attachNode(cubeNode);
  // create a directional light
  DirectionalLight *directionalLight = new DirectionalLight();
  directionalLight->setColor(1.0f, 1.0f, 1.0f);
  directionalLight->setDiffuseIntensity(1.0f);
  directionalLight->setSpecularIntensity(1.0f);
  directionalLight->setDirection(1.0f, -1.0f, -1.0f);
  rootNode->attachLight(directionalLight);
  // create a point light
  PointLight *pointLight = new PointLight();
  pointLight->setColor(1.0f, 0.0f, 1.0f);
  pointLight->setDiffuseIntensity(1.0f);
  pointLight->setSpecularIntensity(0.0f);
  pointLight->setPosition(0.0f, 200.0f, 0.0f);
  pointLight->setRadius(512.0f);
  rootNode->attachLight(pointLight);
  // create camera
  camera = new Camera();
  camera->setPosition(0, 170, 300);
  camera->lookAt(0, 100, 0);
  camera->setAspectRatio(float(width) / float(height));
  // start rendering
  double time = glfwGetTime();
  float timeDiff = 0;
  int frames = 0;
  float fpsTime = 0;
  while (!glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED)) {
    // update time
    timeDiff = glfwGetTime() - time;
    // get time
    time += timeDiff;
    // increase frame counter
    fpsTime += timeDiff;
    frames++;
    // calculate fps
    if (fpsTime >= 1.0) {
      printf("FPS: %f\n", frames / fpsTime);
      frames = 0;
      fpsTime = 0;
    }
    // render one frame
    renderer->renderOneFrame(camera, rootNode);
    // swap front and back rendering buffers
    glfwSwapBuffers();
    // toggle cull face
    if (glfwGetKey(GLFW_KEY_LCTRL))
      glDisable(GL_CULL_FACE);
    else
      glEnable(GL_CULL_FACE);
    // apply animations
    cubeNode->rotate(timeDiff * 60, glm::vec3(1, 1, 1));
    pointLight->setPosition(sinf(time) * 200.0f, 100.f, cosf(time) * 200.0f);
  }
  // clean up
  glfwTerminate();
  
  return 0;
}
