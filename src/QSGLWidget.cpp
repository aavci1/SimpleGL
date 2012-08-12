#include <GL/glew.h>

#include "QSGLWidget.h"

#include "Camera.h"
#include "DirectionalLight.h"
#include "Instance.h"
#include "Material.h"
#include "PointLight.h"
#include "Program.h"
#include "Root.h"
#include "SceneNode.h"
#include "Texture.h"
#include "Viewport.h"
#include "Window.h"

#include <QWheelEvent>

using namespace SimpleGL;

class QSGLWidgetPrivate {
public:
  SimpleGL::Window *window = nullptr;
  QPoint mousePosition;
};

QSGLWidget::QSGLWidget(QWidget *parent) : QGLWidget(parent), d(new QSGLWidgetPrivate()) {
}

QSGLWidget::~QSGLWidget() {
  delete d;
}

void QSGLWidget::initializeGL() {
  // initialize glew
  // TODO: what will happen this line called multiple times?
  glewInit();
  // create associated window
  d->window = Root::instance()->createWindow(this->width(), this->height());
  // create a camera node
  SceneNode *cameraNode = Root::instance()->rootSceneNode()->createChildSceneNode(Vector3f(0.0f, 170.0f, 1500.0f));
  cameraNode->attachObject(d->window->viewports().at(0)->camera());
  // load programs
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
  Material *floorMaterial = Root::instance()->createMaterial("Laminate");
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
  // create meshes
  Root::instance()->createPlane("Plane", 1000, 1000, 10, 10);
  Root::instance()->createSphere("Sphere", 10.0f);
  Root::instance()->createCube("Cube", 50.0f, 50.0f, 50.0f);
  // create floor object
  Instance *floor = Root::instance()->createInstance("");
  floor->setMesh("Plane");
  floor->setMaterial("Laminate");
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
      SceneNode *lightNode = Root::instance()->rootSceneNode()->createChildSceneNode(Vector3f(j * 180.0f, 290.0f, i * 180.0f));
      // create a sphere instance
      Instance *sphereInstance = Root::instance()->createInstance("");
      sphereInstance->setMesh("Sphere");
      sphereInstance->setMaterial("Ceiling");
      lightNode->attachObject(sphereInstance);
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
      // create model instance
      Instance *modelInstance = Root::instance()->createInstance("");
      modelInstance->setMesh("MODEL");
      SceneNode *modelNode = Root::instance()->rootSceneNode()->createChildSceneNode();
      modelNode->setPosition(j * 180.0f, 0.0f, i * 180.0f);
      modelNode->yaw(180.0f);
      // modelNode->pitch(90.0f);
      modelNode->setScale(20, 20, 20);
      modelNode->attachObject(modelInstance);
    }
  }
}

void QSGLWidget::resizeGL(int width, int height) {
  d->window->setSize(width, height);
}

void QSGLWidget::paintGL() {
  Root::instance()->renderOneFrame(100);
}

void QSGLWidget::keyPressEvent(QKeyEvent *e) {

}

void QSGLWidget::keyReleaseEvent(QKeyEvent *e) {

}

void QSGLWidget::mouseMoveEvent(QMouseEvent *e) {
  SceneNode *cameraNode = d->window->viewports().at(0)->camera()->parentSceneNode();
  if (e->buttons() == Qt::RightButton) {
    // pan camera
    cameraNode->moveRelative(Vector3f(d->mousePosition.x() - e->pos().x(), e->pos().y() - d->mousePosition.y(), 0));
  } else if (e->buttons() == Qt::MiddleButton) {
    static float verticalAngle = 0, horizontalAngle = 0;
    verticalAngle += -0.1f * (e->y() - d->mousePosition.y());
    if (verticalAngle > 80)
      verticalAngle = 80;
    else if (verticalAngle < -80)
      verticalAngle = -80;
    horizontalAngle += -0.1f * (e->x() - d->mousePosition.x());
    // set camera orientation
    cameraNode->setOrientation(glm::angleAxis(horizontalAngle, Vector3f(0, 1, 0)) * glm::angleAxis(verticalAngle,  Vector3f(1, 0, 0)));
  }
  // update mouse position
  d->mousePosition = e->pos();
  // update view
  updateGL();
}

void QSGLWidget::mousePressEvent(QMouseEvent *e) {
  // update mouse position
  d->mousePosition = e->pos();
}

void QSGLWidget::mouseReleaseEvent(QMouseEvent *e) {
  // update mouse position
  d->mousePosition = e->pos();
}

void QSGLWidget::wheelEvent(QWheelEvent *e) {
  SceneNode *cameraNode = d->window->viewports().at(0)->camera()->parentSceneNode();
  float altitude = cameraNode->position().y;
  cameraNode->moveRelative(Vector3f(0, 0, -0.4f * e->delta()));
  cameraNode->setPosition(cameraNode->position().x, altitude, cameraNode->position().z);
  // update view
  updateGL();
}
