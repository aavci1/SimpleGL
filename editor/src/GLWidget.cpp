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
  // create a camera
  CameraPtr camera = Root::instance()->createCamera("FpsCamera");
  camera->setNearClipDistance(10);
  camera->setFarClipDistance(10000);
  // create camera node
  SceneNodePtr cameraNode = Root::instance()->createSceneNode();
  Root::instance()->rootSceneNode()->attachNode(cameraNode);
  cameraNode->setPosition(Vector3f(0.0f, 1000.0f, 3500.0f));
  cameraNode->pitch(-20);
  // attach camera to the node
  cameraNode->attachObject(camera);
  // create a window
  window = Root::instance()->createWindow(width(), height());
  // create a viewport
  window->createViewport(camera);
  // load scripts
  Root::instance()->loadScript("media/Core.script");
  // create meshes
  Root::instance()->createPlane("Plane", 10000, 10000, 10, 10);
  Root::instance()->createSphere("Sphere", 10.0f);
  Root::instance()->createCube("Cube", 50.0f, 50.0f, 50.0f);
  // create floor object
  SceneNodePtr floorNode = Root::instance()->createSceneNode();
  Root::instance()->rootSceneNode()->attachNode(floorNode);
  floorNode->attachObject(Root::instance()->createInstance("Plane", "Laminate"));
  // add a directional light
  shared_ptr<DirectionalLight> directionalLight = static_pointer_cast<DirectionalLight>(Root::instance()->createLight("Light/Directional"));
  directionalLight->setColor(0.1f, 0.1f, 0.1f);
  directionalLight->setDiffuseIntensity(1.0f);
  directionalLight->setSpecularIntensity(0.0f);
  directionalLight->setDirection(0, -1, -1);
  Root::instance()->rootSceneNode()->attachObject(directionalLight);
  // create lots of point lights
  srand(0);
  int M = 25, N = 40;
  for (int i = 0; i < M ; ++i) {
    for (int j = 0; j < N; ++j) {
      int x = (i - M / 2) * 180.0f, z = (j - N / 2) * 180.0f;
      // create light node
      SceneNodePtr lightNode = Root::instance()->createSceneNode();
      Root::instance()->rootSceneNode()->attachNode(lightNode);
      lightNode->setPosition(x, 290.0f, z);
      // attach a light
      shared_ptr<PointLight> light = static_pointer_cast<PointLight>(Root::instance()->createLight("Light/Point"));
      //shared_ptr<SpotLight> light = static_pointer_cast<SpotLight>(Root::instance()->createLight("Light/Spot"));
      //light->setInnerAngle(10);
      //light->setOuterAngle(40);
      //lightNode->pitch(-90, TS_WORLD);
      light->setColor(float(rand()) / RAND_MAX, float(rand()) / RAND_MAX, float(rand()) / RAND_MAX);
      light->setDiffuseIntensity(1.0f);
      light->setSpecularIntensity(1.0f);
      light->setAttenuation(400.0f);
      lightNode->attachObject(light);
      // create an instance
      InstancePtr instance = Root::instance()->createInstance("MODEL", "");
      // add model to the scene
      SceneNodePtr modelNode = Root::instance()->createSceneNode();
      Root::instance()->rootSceneNode()->attachNode(modelNode);
      modelNode->setPosition(x, 0.0f, z);
      modelNode->setScale(3.0f, 3.0f, 3.0f);
      modelNode->attachObject(instance);
    }
  }
  Root::instance()->load("MODEL", "media/models/bob.model");
}

void GLWidget::resizeGL(int width, int height) {
  if (window)
    window->setSize(width, height);
}

void GLWidget::paintGL() {
  // move camera
  if (moving) {
    CameraPtr camera = Root::instance()->retrieveCamera("FpsCamera");
    if (!camera)
      return;
    SceneNode *cameraNode = camera->parent();
    if (!cameraNode)
      return;
    float altitude = cameraNode->position().y;
    QPoint diff = referencePosition - mousePosition;
    cameraNode->moveRelative(-0.5f * diff.x(), 0, -0.5f * diff.y());
    cameraNode->setPosition(cameraNode->position().x, altitude, cameraNode->position().z);
  }

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
  // start moving
  if (e->button() == Qt::LeftButton) {
    referencePosition = e->pos();
    moving = true;
  }
  // update view
  updateGL();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *e) {
  // update mouse position
  mousePosition = e->pos();
  if (e->button() == Qt::LeftButton) {
    referencePosition = QPoint(0, 0);
    moving = false;
  }
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