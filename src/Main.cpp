#include "Camera.h"
#include "DirectionalLight.h"
#include "Instance.h"
#include "Material.h"
#include "Mesh.h"
#include "PointLight.h"
#include "Program.h"
#include "Root.h"
#include "SceneNode.h"
#include "SpotLight.h"
#include "Texture.h"
#include "Viewport.h"
#include "Window.h"

using namespace SimpleGL;

void loadMaterials() {
  Program *pointLightProgram = Root::instance()->createProgram("PointLight");
  if (!pointLightProgram->loadShaderFromPath(ST_VERTEX, "media/point_light_vp.glsl")) std::cerr << pointLightProgram->errorMessage() << std::endl;
  if (!pointLightProgram->loadShaderFromPath(ST_FRAGMENT, "media/point_light_fp.glsl")) std::cerr << pointLightProgram->errorMessage() << std::endl;
  if (!pointLightProgram->link()) std::cerr << pointLightProgram->errorMessage() << std::endl;
  // spot light program
  Program *spotLightProgram = Root::instance()->createProgram("SpotLight");
  if (!spotLightProgram->loadShaderFromPath(ST_VERTEX, "media/spot_light_vp.glsl")) std::cerr << spotLightProgram->errorMessage() << std::endl;
  if (!spotLightProgram->loadShaderFromPath(ST_FRAGMENT, "media/spot_light_fp.glsl")) std::cerr << spotLightProgram->errorMessage() << std::endl;
  if (!spotLightProgram->link()) std::cerr << spotLightProgram->errorMessage() << std::endl;
  // directional light program
  Program *directionalLightProgram = Root::instance()->createProgram("DirectionalLight");
  if (!directionalLightProgram->loadShaderFromPath(ST_VERTEX, "media/directional_light_vp.glsl")) std::cerr << directionalLightProgram->errorMessage() << std::endl;
  if (!directionalLightProgram->loadShaderFromPath(ST_FRAGMENT, "media/directional_light_fp.glsl")) std::cerr << directionalLightProgram->errorMessage() << std::endl;
  if (!directionalLightProgram->link()) std::cerr << directionalLightProgram->errorMessage() << std::endl;
  // load programs
  Program *program = Root::instance()->createProgram("Textured");
  if (!program->loadShaderFromPath(ST_VERTEX, "media/textured_vp.glsl")) std::cerr << program->errorMessage() << std::endl;
  if (!program->loadShaderFromPath(ST_FRAGMENT, "media/textured_fp.glsl")) std::cerr << program->errorMessage() << std::endl;
  if (!program->link()) std::cerr << program->errorMessage() << std::endl;
  // load textures
  Root::instance()->createTexture("Laminate", "media/laminate.jpg");
  Root::instance()->createTexture("Ceiling", "media/ceiling.jpg");
  Root::instance()->createTexture("Ebony", "media/ebony.jpg");
  // create materials
  Material *floorMaterial = Root::instance()->createMaterial("Floor");
  floorMaterial->setProgram("Textured");
  floorMaterial->addTexture("Laminate");
  // load materials
  Material *ceilingMaterial = Root::instance()->createMaterial("Ceiling");
  ceilingMaterial->setProgram("Textured");
  ceilingMaterial->addTexture("Ceiling");
  // load materials
  Material *ebonyMaterial = Root::instance()->createMaterial("Ebony");
  ebonyMaterial->setProgram("Textured");
  ebonyMaterial->addTexture("Ebony");
}

void createScene(int argc, char **argv) {
  // create meshes
  Root::instance()->createPlane("Plane", 1000, 1000, 10, 10);
  Root::instance()->createSphere("Sphere", 10.0f);
  Root::instance()->createCube("Cube", 50.0f, 50.0f, 50.0f);
  // create floor object
  Instance *floor = Root::instance()->createInstance("");
  floor->setMesh("Plane");
  floor->setMaterial("Floor");
  SceneNode *floorNode = Root::instance()->rootSceneNode()->createChildSceneNode();
  floorNode->attachObject(floor);
  // create ceiling object
  Instance *ceiling = Root::instance()->createInstance("");
  ceiling->setMesh("Plane");
  ceiling->setMaterial("Ceiling");
  SceneNode *ceilingNode = Root::instance()->rootSceneNode()->createChildSceneNode();
  ceilingNode->setPosition(0.0f, 300.0f, 0.0f);
  ceilingNode->roll(180.0f);
  ceilingNode->attachObject(ceiling);
  // create a cube
  Instance *cube = Root::instance()->createInstance("");
  cube->setMesh("Cube");
  cube->setMaterial("Ebony");
  SceneNode *cubeNode = Root::instance()->rootSceneNode()->createChildSceneNode();
  cubeNode->setPosition(0.0f, 150.0f, 0.0f);
  cubeNode->attachObject(cube);
  // add a directional light
  DirectionalLight *directionalLight = static_cast<DirectionalLight *>(Root::instance()->createLight(LT_DIRECTIONAL));
  directionalLight->setColor(1.0f, 1.0f, 1.0f);
  directionalLight->setDiffuseIntensity(1.0f);
  directionalLight->setSpecularIntensity(0.0f);
  directionalLight->setDirection(0, -1, -1);
  Root::instance()->rootSceneNode()->attachObject(directionalLight);
  // create lots of point lights
  srand(0);
  for (int i = -5; i <= 5; ++i) {
    for (int j = -5; j <= 5; ++j) {
      SceneNode *lightNode = Root::instance()->rootSceneNode()->createChildSceneNode(Vector3f(j * 180, 290.0f, i * 180));
      // create a sphere instance
      Instance *instance = Root::instance()->createInstance("");
      instance->setMesh("Sphere");
      instance->setMaterial("Ceiling");
      lightNode->attachObject(instance);
      // create a light
      PointLight *light = static_cast<PointLight *>(Root::instance()->createLight(LT_POINT));
      // SpotLight *light = static_cast<SpotLight *>(Root::instance()->createLight(LT_SPOT));
      // light->setInnerAngle(10);
      // light->setOuterAngle(10);
      light->setColor(float(rand()) / RAND_MAX, float(rand()) / RAND_MAX, float(rand()) / RAND_MAX);
      light->setDiffuseIntensity(1.0f);
      light->setSpecularIntensity(1.0f);
      light->setAttenuation(400.0f);
      lightNode->pitch(-90, TS_WORLD);
      lightNode->attachObject(light);
    }
  }
}

void navigateCamera(SceneNode *cameraNode, long millis) {
  // camera angles
  static float verticalAngle = 0, horizontalAngle = 0;
  static Vector2i mousePosition = Root::instance()->mousePosition();
  // update camera position
  float height = cameraNode->position().y;
  if (Root::instance()->isKeyDown('W'))
    cameraNode->moveRelative(0.0f, 0.0f, -0.25f * millis);
  if (Root::instance()->isKeyDown('A'))
    cameraNode->moveRelative(-0.25f * millis, 0.0f, 0.0f);
  if (Root::instance()->isKeyDown('S'))
    cameraNode->moveRelative(0.0f, 0.0f, +0.25f * millis);
  if (Root::instance()->isKeyDown('D'))
    cameraNode->moveRelative(+0.25f * millis, 0.0f, 0.0f);
  // reset camera height
  cameraNode->setPosition(cameraNode->position().x, height, cameraNode->position().z);
  // update camera orientation
  Vector2i position = Root::instance()->mousePosition();
  // rotate camera
  verticalAngle += -0.01f * millis * (position.y - mousePosition.y);
  if (verticalAngle > 80)
    verticalAngle = 80;
  else if (verticalAngle < -80)
    verticalAngle = -80;
  horizontalAngle += -0.01f * millis * (position.x - mousePosition.x);
  // set camera orientation
  cameraNode->setOrientation(glm::angleAxis(horizontalAngle, Vector3f(0, 1, 0)) * glm::angleAxis(verticalAngle,  Vector3f(1, 0, 0)));
  // update mouse coordinates
  mousePosition = position;
}

int main(int argc, char **argv) {
  Root::initialize();
  // create a render window
  Window *window = Root::instance()->createWindow(1920, 1080, true);
  // create a camera node
  SceneNode *cameraNode = Root::instance()->rootSceneNode()->createChildSceneNode(Vector3f(0.0f, 170.0f, 1500.0f));
  cameraNode->attachObject(window->viewports().at(0)->camera());
  // load materials
  loadMaterials();
  // create scene
  createScene(argc, argv);
  // start rendering
  while (window->isOpen()) {
    Root::instance()->fps();
    // render scene
    long millis = Root::instance()->renderOneFrame();
    // navigate camera
    navigateCamera(cameraNode, millis);
  }
  // deinitialize SimpleGL
  Root::destroy();
  // return success
  return 0;
}
