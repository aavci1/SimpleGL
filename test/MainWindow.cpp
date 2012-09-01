#include "MainWindow.h"

#include "Animation.h"
#include "AssimpImporter.h"
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
#include "SubMesh.h"
#include "Window.h"
#include "Viewport.h"

#include <QKeyEvent>
#include <QMouseEvent>
#include <QTimer>
#include <QWheelEvent>

using namespace SimpleGL;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  setupUi(this);
  // connect signals
  connect(widget, SIGNAL(initialized()), this, SLOT(initialized()));
  connect(widget, SIGNAL(keyPressed(QKeyEvent*)), this, SLOT(keyPressed(QKeyEvent*)));
  connect(widget, SIGNAL(keyReleased(QKeyEvent*)), this, SLOT(keyReleased(QKeyEvent*)));
  connect(widget, SIGNAL(mouseMoved(QMouseEvent*)), this, SLOT(mouseMoved(QMouseEvent*)));
  connect(widget, SIGNAL(mousePressed(QMouseEvent*)), this, SLOT(mousePressed(QMouseEvent*)));
  connect(widget, SIGNAL(mouseReleased(QMouseEvent*)), this, SLOT(mouseReleased(QMouseEvent*)));
  connect(widget, SIGNAL(wheelMoved(QWheelEvent*)), this, SLOT(wheelMoved(QWheelEvent*)));
  timer = new QTimer();
  timer->setInterval(10);
  connect(timer, SIGNAL(timeout()), widget, SLOT(updateGL()));
  timer->start();
}

MainWindow::~MainWindow() {
  delete timer;
  // deinitialize SimpleGL
  SimpleGL::Root::destroy();
}

void MainWindow::initialized() {
  // create a camera
  camera = Root::instance()->createCamera();
  // create camera node
  cameraNode = Root::instance()->rootSceneNode()->createChildSceneNode(Vector3f(0.0f, 170.0f, 300.0f));
  cameraNode->pitch(-10);
  cameraNode->attachObject(camera);
  // create viewport
  widget->sglWindow()->createViewport(camera);
  // load programs
  Program *pointLightProgram = Root::instance()->createProgram("PointLight");
  if (!pointLightProgram->loadShaderFromPath(ST_VERTEX, "media/point_light_vp.glsl")) cerr << pointLightProgram->log() << endl;
  if (!pointLightProgram->loadShaderFromPath(ST_FRAGMENT, "media/point_light_fp.glsl")) cerr << pointLightProgram->log() << endl;
  if (!pointLightProgram->link()) cerr << pointLightProgram->log() << endl;
  // spot light program
  Program *spotLightProgram = Root::instance()->createProgram("SpotLight");
  if (!spotLightProgram->loadShaderFromPath(ST_VERTEX, "media/spot_light_vp.glsl")) cerr << spotLightProgram->log() << endl;
  if (!spotLightProgram->loadShaderFromPath(ST_FRAGMENT, "media/spot_light_fp.glsl")) cerr << spotLightProgram->log() << endl;
  if (!spotLightProgram->link()) cerr << spotLightProgram->log() << endl;
  // directional light program
  Program *directionalLightProgram = Root::instance()->createProgram("DirectionalLight");
  if (!directionalLightProgram->loadShaderFromPath(ST_VERTEX, "media/directional_light_vp.glsl")) cerr << directionalLightProgram->log() << endl;
  if (!directionalLightProgram->loadShaderFromPath(ST_FRAGMENT, "media/directional_light_fp.glsl")) cerr << directionalLightProgram->log() << endl;
  if (!directionalLightProgram->link()) cerr << directionalLightProgram->log() << endl;
  // load texturing program
  Program *textured = Root::instance()->createProgram("Textured");
  if (!textured->loadShaderFromPath(ST_VERTEX, "media/textured_vp.glsl")) cerr << textured->log() << endl;
  if (!textured->loadShaderFromPath(ST_FRAGMENT, "media/textured_fp.glsl")) cerr << textured->log() << endl;
  if (!textured->link()) cerr << textured->log() << endl;
  // load skinning program
  Program *skinned = Root::instance()->createProgram("Skinned");
  if (!skinned->loadShaderFromPath(ST_VERTEX, "media/skinned_vp.glsl")) cerr << skinned->log() << endl;
  if (!skinned->loadShaderFromPath(ST_FRAGMENT, "media/skinned_fp.glsl")) cerr << skinned->log() << endl;
  if (!skinned->link()) cerr << skinned->log() << endl;
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
  SceneNode *floorNode = Root::instance()->rootSceneNode()->createChildSceneNode();
  floorNode->attachObject(Root::instance()->createInstance("Plane", "Laminate"));
  // create ceiling object
  SceneNode *ceilingNode = Root::instance()->rootSceneNode()->createChildSceneNode();
  ceilingNode->setPosition(0.0f, 300.0f, 0.0f);
  ceilingNode->roll(180.0f);
  ceilingNode->attachObject(Root::instance()->createInstance("Plane", "Ceiling"));
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
      // create light node
      SceneNode *lightNode = Root::instance()->rootSceneNode()->createChildSceneNode(Vector3f(j * 180.0f, 290.0f, i * 180.0f));
      // attach a sphere
      lightNode->attachObject(Root::instance()->createInstance("Sphere", "Ceiling"));
      // attach a light
      PointLight *light = static_cast<PointLight *>(Root::instance()->createLight(LT_POINT));
      // SpotLight *light = static_cast<SpotLight *>(Root::instance()->createLight(LT_SPOT));
      // light->setInnerAngle(10);
      // light->setOuterAngle(40);
      light->setColor(float(rand()) / RAND_MAX, float(rand()) / RAND_MAX, float(rand()) / RAND_MAX);
      light->setDiffuseIntensity(1.0f);
      light->setSpecularIntensity(1.0f);
      light->setAttenuation(400.0f);
      // lightNode->pitch(-90, TS_WORLD); // needed for spots
      lightNode->attachObject(light);
    }
  }
  // import
  AssimpImporter *importer = new AssimpImporter();
  Mesh *mesh = importer->import("MODEL", "/home/aavci/Documents/SimpleGL/markus/markus.dae");
  mesh->subMeshes().at(0)->setMaterial("Markus");
  delete importer;
  // save model
  Root::instance()->save("MODEL", "markus.sglm");
  // load model
  Root::instance()->load("MARKUS", "markus.sglm");
  // create texture
  Root::instance()->createTexture("markus_diffuse", "markus_diffuse.png");
  // create material
  Material *markusMaterial = Root::instance()->createMaterial("Markus");
  markusMaterial->setProgram("Skinned");
  markusMaterial->addTexture("markus_diffuse");
  // add model to the scene
  SceneNode *modelNode = Root::instance()->rootSceneNode()->createChildSceneNode();
  modelNode->setScale(3, 3, 3);
  modelNode->attachObject(Root::instance()->createInstance("MARKUS", ""));
}

void MainWindow::keyPressed(QKeyEvent *e) {

}

void MainWindow::keyReleased(QKeyEvent *e) {

}

void MainWindow::mouseMoved(QMouseEvent *e) {
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
  widget->updateGL();
}

void MainWindow::mousePressed(QMouseEvent *e) {
  // update mouse position
  mousePosition = e->pos();
}

void MainWindow::mouseReleased(QMouseEvent *e) {
  // update mouse position
  mousePosition = e->pos();
}

void MainWindow::wheelMoved(QWheelEvent *e) {
  float altitude = cameraNode->position().y;
  cameraNode->moveRelative(Vector3f(0, 0, -0.4f * e->delta()));
  cameraNode->setPosition(cameraNode->position().x, altitude, cameraNode->position().z);
  // update view
  widget->updateGL();
}
