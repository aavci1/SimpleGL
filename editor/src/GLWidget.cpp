#include "GLWidget.h"

#include "AssimpImporter.h"
#include "Animation.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "Instance.h"
#include "Material.h"
#include "Mesh.h"
#include "Model.h"
#include "PointLight.h"
#include "Program.h"
#include "Root.h"
#include "SceneNode.h"
#include "SpotLight.h"
#include "Types.h"
#include "Window.h"
#include "Viewport.h"

#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>

using namespace SimpleGL;

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent) {
}

GLWidget::~GLWidget() {
  // deinitialize SimpleGL
  SimpleGL::destroy();
}

void GLWidget::initializeGL() {
  // initialize SimpleGL
  SimpleGL::initialize();
  // create window
  window = Root::instance()->createWindow(width(), height());
  // create camera node
  SceneNodePtr cameraNode = Root::instance()->createSceneNode();
  Root::instance()->rootSceneNode()->attachNode(cameraNode);
  cameraNode->setPosition(Vector3f(0.0f, 170.0f, 1000.0f));
  cameraNode->pitch(-10);
  // create a camera
  CameraPtr camera = Root::instance()->createCamera("FpsCamera");
  cameraNode->attachObject(camera);
  // create a viewport
  window->createViewport(camera);
  // create default program
  ProgramPtr defaultProgram = Root::instance()->createProgram("Default");
  if (!defaultProgram->loadShaderFromPath(ST_VERTEX, "media/Default.vert")) cerr << defaultProgram->log() << endl;
  if (!defaultProgram->loadShaderFromPath(ST_FRAGMENT, "media/Default.frag")) cerr << defaultProgram->log() << endl;
  if (!defaultProgram->link()) cerr << defaultProgram->log() << endl;
  // create default material
  MaterialPtr defaultMaterial = Root::instance()->createMaterial("Default");
  defaultMaterial->setProgram("Default");
  // point light program
  ProgramPtr pointLightProgram = Root::instance()->createProgram("Light/Point");
  if (!pointLightProgram->loadShaderFromPath(ST_VERTEX, "media/point_light_vp.glsl")) cerr << pointLightProgram->log() << endl;
  if (!pointLightProgram->loadShaderFromPath(ST_FRAGMENT, "media/point_light_fp.glsl")) cerr << pointLightProgram->log() << endl;
  if (!pointLightProgram->link()) cerr << pointLightProgram->log() << endl;
  // spot light program
  ProgramPtr spotLightProgram = Root::instance()->createProgram("Light/Spot");
  if (!spotLightProgram->loadShaderFromPath(ST_VERTEX, "media/spot_light_vp.glsl")) cerr << spotLightProgram->log() << endl;
  if (!spotLightProgram->loadShaderFromPath(ST_FRAGMENT, "media/spot_light_fp.glsl")) cerr << spotLightProgram->log() << endl;
  if (!spotLightProgram->link()) cerr << spotLightProgram->log() << endl;
  // directional light program
  ProgramPtr directionalLightProgram = Root::instance()->createProgram("Light/Directional");
  if (!directionalLightProgram->loadShaderFromPath(ST_VERTEX, "media/directional_light_vp.glsl")) cerr << directionalLightProgram->log() << endl;
  if (!directionalLightProgram->loadShaderFromPath(ST_FRAGMENT, "media/directional_light_fp.glsl")) cerr << directionalLightProgram->log() << endl;
  if (!directionalLightProgram->link()) cerr << directionalLightProgram->log() << endl;
  // load texturing program
  ProgramPtr textured = Root::instance()->createProgram("Textured");
  if (!textured->loadShaderFromPath(ST_VERTEX, "media/textured_vp.glsl")) cerr << textured->log() << endl;
  if (!textured->loadShaderFromPath(ST_FRAGMENT, "media/textured_fp.glsl")) cerr << textured->log() << endl;
  if (!textured->link()) cerr << textured->log() << endl;
  // load skinning program
  ProgramPtr skinned = Root::instance()->createProgram("Skinned");
  if (!skinned->loadShaderFromPath(ST_VERTEX, "media/skinned_vp.glsl")) cerr << skinned->log() << endl;
  if (!skinned->loadShaderFromPath(ST_FRAGMENT, "media/skinned_fp.glsl")) cerr << skinned->log() << endl;
  if (!skinned->link()) cerr << skinned->log() << endl;
  // create materials
  MaterialPtr floorMaterial = Root::instance()->createMaterial("Laminate");
  floorMaterial->setProgram("Textured");
  floorMaterial->addTexture("media/laminate.jpg");
  // load materials
  MaterialPtr ceilingMaterial = Root::instance()->createMaterial("Ceiling");
  ceilingMaterial->setProgram("Textured");
  ceilingMaterial->addTexture("media/ceiling.jpg");
  // load materials
  MaterialPtr ebonyMaterial = Root::instance()->createMaterial("Ebony");
  ebonyMaterial->setProgram("Textured");
  ebonyMaterial->addTexture("media/ebony.jpg");
  // create meshes
  Root::instance()->createPlane("Plane", 1000, 1000, 10, 10);
  Root::instance()->createSphere("Sphere", 10.0f);
  Root::instance()->createCube("Cube", 50.0f, 50.0f, 50.0f);
  // create floor object
  SceneNodePtr floorNode = Root::instance()->createSceneNode();
  Root::instance()->rootSceneNode()->attachNode(floorNode);
  floorNode->attachObject(Root::instance()->createInstance("Plane", "Laminate"));
  // create ceiling object
  SceneNodePtr ceilingNode = Root::instance()->createSceneNode();
  Root::instance()->rootSceneNode()->attachNode(ceilingNode);
  ceilingNode->setPosition(0.0f, 300.0f, 0.0f);
  ceilingNode->roll(180.0f);
  ceilingNode->attachObject(Root::instance()->createInstance("Plane", "Ceiling"));
  // add a directional light
  shared_ptr<DirectionalLight> directionalLight = static_pointer_cast<DirectionalLight>(Root::instance()->createLight("Light/Directional"));
  directionalLight->setColor(1.0f, 1.0f, 1.0f);
  directionalLight->setDiffuseIntensity(1.0f);
  directionalLight->setSpecularIntensity(0.0f);
  directionalLight->setDirection(0, -1, -1);
  Root::instance()->rootSceneNode()->attachObject(directionalLight);
  // create lots of point lights
  srand(0);
  for (int i = -5; i <= 5; ++i) {
    for (int j = -5; j <= 5; ++j) {
      // create light node
      SceneNodePtr lightNode = Root::instance()->createSceneNode();
      Root::instance()->rootSceneNode()->attachNode(lightNode);
      lightNode->setPosition(j * 180.0f, 290.0f, i * 180.0f);
      // attach a sphere
      lightNode->attachObject(Root::instance()->createInstance("Sphere", "Ceiling"));
      // attach a light
      shared_ptr<PointLight> light = static_pointer_cast<PointLight>(Root::instance()->createLight("Light/Point"));
      // shared_ptr<SpotLight> light = static_pointer_cast<SpotLight>(Root::instance()->createLight("Light/Spot"));
      // light->setInnerAngle(10);
      // light->setOuterAngle(40);
      light->setColor(float(rand()) / RAND_MAX, float(rand()) / RAND_MAX, float(rand()) / RAND_MAX);
      light->setDiffuseIntensity(1.0f);
      light->setSpecularIntensity(1.0f);
      light->setAttenuation(400.0f);
      // lightNode->pitch(-90, TS_WORLD); // needed for spots
      lightNode->attachObject(light);
      // create an instance
      InstancePtr instance = Root::instance()->createInstance("MODEL", "");
      // add model to the scene
      SceneNodePtr modelNode = Root::instance()->createSceneNode();
      Root::instance()->rootSceneNode()->attachNode(modelNode);
      modelNode->setPosition(j * 180.0f, 0.0f, i * 180.0f);
      modelNode->setScale(3.0f, 3.0f, 3.0f);
      modelNode->attachObject(instance);
    }
  }
}

void GLWidget::resizeGL(int width, int height) {
  if (window)
    window->setSize(width, height);
}

void GLWidget::paintGL() {
  if (window)
    window->update();
}

void GLWidget::keyPressEvent(QKeyEvent *e) {
  // update view
  updateGL();
}

void GLWidget::keyReleaseEvent(QKeyEvent *e) {
  // update view
  updateGL();
}

void GLWidget::mouseMoveEvent(QMouseEvent *e) {
  CameraPtr camera = Root::instance()->retrieveCamera("FpsCamera");
  if (!camera)
    return;
  SceneNode *cameraNode = camera->parent();
  if (!cameraNode)
    return;
  if (e->buttons() == Qt::RightButton) {
    // pan camera
    cameraNode->moveRelative(Vector3f(mousePosition.x() - e->pos().x(), e->pos().y() - mousePosition.y(), 0));
  } else if (e->buttons() == Qt::MiddleButton) {
    static float verticalAngle = 0, horizontalAngle = 0;
    verticalAngle += -0.1f * (e->y() - mousePosition.y());
    if (verticalAngle > 80)
      verticalAngle = 80;
    else if (verticalAngle < -80)
      verticalAngle = -80;
    horizontalAngle += -0.1f * (e->x() - mousePosition.x());
    // set camera orientation
    cameraNode->setOrientation(glm::angleAxis(horizontalAngle, Vector3f(0, 1, 0)) * glm::angleAxis(verticalAngle,  Vector3f(1, 0, 0)));
  }
  // update mouse position
  mousePosition = e->pos();
  // update view
  updateGL();
}

void GLWidget::mousePressEvent(QMouseEvent *e) {
  // update mouse position
  mousePosition = e->pos();
  // update view
  updateGL();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *e) {
  // update mouse position
  mousePosition = e->pos();
  // update view
  updateGL();
}

void GLWidget::wheelEvent(QWheelEvent *e) {
  CameraPtr camera = Root::instance()->retrieveCamera("FpsCamera");
  if (!camera)
    return;
  SceneNode *cameraNode = camera->parent();
  if (!cameraNode)
    return;
  float altitude = cameraNode->position().y;
  cameraNode->moveRelative(Vector3f(0, 0, -0.4f * e->delta()));
  cameraNode->setPosition(cameraNode->position().x, altitude, cameraNode->position().z);
  // update view
  updateGL();
}