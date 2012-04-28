#include "Camera.h"
#include "DirectionalLight.h"
#include "Material.h"
#include "MaterialManager.h"
#include "Mesh.h"
#include "MeshManager.h"
#include "SceneNode.h"
#include "PointLight.h"
#include "Renderer.h"
#include "SpotLight.h"

#include <glm/glm.hpp>

#include <GL/glew.h>
#include <GL/glfw.h>

using namespace SimpleGL;

Camera *camera;
Renderer *renderer;

void GLFWCALL resize(int width, int height) {
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
    std::cerr << "error: can not initialize glfw." << std::endl;
    return 1;
  }
  // request opengl version
  glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
  glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
  // request opengl profile
  glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // try to open the window
  if (!glfwOpenWindow(1920, 1080, 8, 8, 8, 0, 24, 0, GLFW_FULLSCREEN)) {
    std::cerr << "error: cannot open glfw window." << std::endl;
    glfwTerminate();
    return 1;
  }
  // set window title
  glfwSetWindowTitle("SimpleGL");
  // set window resize call back, must be set after window creation
  glfwSetWindowSizeCallback(resize);
  // initialize glew, must be after window creation
  if (glewInit() != GLEW_OK) {
    std::cerr << "error: can not initialize glew." << std::endl;
    return 1;
  }
  // get window width and height
  int width = 0, height = 0;
  glfwGetWindowSize(&width, &height);
  // create renderer
  // WARN: if renderer is initialized further down, some strange artifacts occur
  renderer = new Renderer(width, height);
  // create root node
  SceneNode *rootNode = new SceneNode();
  // register directional light material
  Material *directionalLightMaterial = MaterialManager::instance()->getMaterialByLightType(LT_DIRECTIONAL);
  directionalLightMaterial->setProgram("media/directionalLight.vert", "media/directionalLight.frag");
  // register point light material
  Material *pointLightMaterial = MaterialManager::instance()->getMaterialByLightType(LT_POINT);
  pointLightMaterial->setProgram("media/pointLight.vert", "media/pointLight.frag");
  // register spot light material
  Material *spotLightMaterial = MaterialManager::instance()->getMaterialByLightType(LT_SPOT);
  spotLightMaterial->setProgram("media/spotLight.vert", "media/spotLight.frag");
  // create laminate material
  Material *laminateMaterial = MaterialManager::instance()->getMaterialByName("Laminate");
  laminateMaterial->setProgram("media/textured.vert", "media/textured.frag");
  laminateMaterial->addTexture("media/laminate.jpg");
  // create ceramics material
  Material *ebonyMaterial = MaterialManager::instance()->getMaterialByName("Ebony");
  ebonyMaterial->setProgram("media/textured.vert", "media/textured.frag");
  ebonyMaterial->addTexture("media/ebony.jpg");
  // create ceiling material
  Material *ceilingMaterial = MaterialManager::instance()->getMaterialByName("Ceiling");
  ceilingMaterial->setProgram("media/textured.vert", "media/textured.frag");
  ceilingMaterial->addTexture("media/ceiling.jpg");
  // create floor
  Mesh *floor = MeshManager::instance()->createPlane(1000, 1000, 10, 10);
  floor->setMaterialName("Laminate");
  SceneNode *floorNode = rootNode->createChildNode();
  floorNode->attach(floor);
  // create ceiling
  Mesh *ceiling = MeshManager::instance()->createPlane(1000, 1000, 10, 10);
  ceiling->setMaterialName("Ceiling");
  SceneNode *ceilingNode = rootNode->createChildNode();
  ceilingNode->attach(ceiling);
  ceilingNode->setPosition(0.0f, 300.0f, 0.0f);
  ceilingNode->roll(180);
  // load a model
  Mesh *model = 0;
  if (argc > 1)
    model = MeshManager::instance()->loadMesh(argv[1]);
  // add a directional light
  DirectionalLight *directionalLight = new DirectionalLight();
  directionalLight->setColor(1.0f, 1.0f, 1.0f);
  directionalLight->setDiffuseIntensity(1.0f);
  directionalLight->setSpecularIntensity(1.0f);
  directionalLight->setDirection(0, -1, -1);
  rootNode->attach(directionalLight);
  // add lots of point lights
  srand(glfwGetTime() * 1000);
  for (int i = -5; i <= 5; ++i) {
    for (int j = -5; j <= 5; ++j) {
      Mesh *sphere = MeshManager::instance()->createSphere(10.0f);
      sphere->setMaterialName("Ceiling");
      SceneNode *lightNode = rootNode->createChildNode(glm::vec3(j * 180, 290.0f, i * 180));
      lightNode->attach(sphere);
      // create a light
      PointLight *light = new PointLight();
      // SpotLight *light = new SpotLight();
      // light->setInnerAngle(10);
      // light->setOuterAngle(5);
      // light->pitch(-90, TS_WORLD);
      light->setColor(float(rand()) / RAND_MAX, float(rand()) / RAND_MAX, float(rand()) / RAND_MAX);
      light->setDiffuseIntensity(1.0f);
      light->setSpecularIntensity(1.0f);
      light->setPosition(j * 180, 290.0f, i * 180);
      light->setAttenuation(400.0f);
      rootNode->attach(light);
      if (model) {
        SceneNode *node = rootNode->createChildNode(glm::vec3(j * 180, 0.0f, i * 180));
        node->yaw(180.0f);
        node->attach(model);
      }
    }
  }
  // create camera
  camera = new Camera();
  camera->setPosition(0, 170, 1500);
  camera->lookAt(0, 130, 0);
  camera->setAspectRatio(float(width) / float(height));
  // mouse position
  int mouseX = 0, mouseY = 0;
  glfwGetMousePos(&mouseX, &mouseY);
  // start rendering
  double time = glfwGetTime();
  float timeDiff = 0;
  int frames = 0;
  float fpsTime = 0;
  float verticalAngle = 0, horizontalAngle = 0;
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
      std::cout << "FPS: " << int(frames / fpsTime) << std::endl;
      frames = 0;
      fpsTime = 0;
    }
    // render one frame
    renderer->renderOneFrame(camera, rootNode);
    // swap front and back rendering buffers
    glfwSwapBuffers();
    // get mouse coordinates
    int x, y;
    glfwGetMousePos(&x, &y);
    // rotate camera
    verticalAngle += -5.0f * timeDiff * (y - mouseY);
    if (verticalAngle > 80)
      verticalAngle = 80;
    else if (verticalAngle < -80)
      verticalAngle = -80;
    horizontalAngle += -5.0f * timeDiff * (x - mouseX);
    // set camera orientation
    camera->setOrientation(glm::angleAxis(horizontalAngle, glm::vec3(0, 1, 0)) * glm::angleAxis(verticalAngle,  glm::vec3(1, 0, 0)));
    // save mouse coordinates
    mouseX = x;
    mouseY = y;
    // move camera
    float height = camera->position().y;
    if (glfwGetKey('W'))
      camera->moveRelative(0.0f, 0.0f, -250.0f * timeDiff);
    if (glfwGetKey('A'))
      camera->moveRelative(-250.0f * timeDiff, 0.0f, 0.0f);
    if (glfwGetKey('S'))
      camera->moveRelative(0.0f, 0.0f, +250.0f * timeDiff);
    if (glfwGetKey('D'))
      camera->moveRelative(+250.0f * timeDiff, 0.0f, 0.0f);
    // reset camera height
    camera->setPosition(camera->position().x, height, camera->position().z);
    // save screenshot
    if (glfwGetKey(GLFW_KEY_F8))
      renderer->saveScreenshot("screenshot.jpg");
  }
  // clean up
  glfwTerminate();
  return 0;
}
