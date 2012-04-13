#include "DeferredRenderer.h"
#include "Camera.h"
#include "CubeMesh.h"
#include "Light.h"
#include "Node.h"
#include "PlaneMesh.h"
#include "SphereMesh.h"

#include <glm/glm.hpp>

#include <GL/glew.h>
#include <GL/glfw.h>

using namespace SimpleGL;

Camera *camera;

void GLFWCALL resize(int width, int height) {
  //   printf("new size: %dx%d\n", width, height);
  glViewport(0, 0, (GLsizei) width, (GLsizei) height);
  // update perspective
  if (camera)
    camera->setAspectRatio(float(width) / float(height));
  // TODO: update renderer buffers' size
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
  // set color to clear background
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glEnable(GL_CULL_FACE);
  glEnable(GL_TEXTURE_2D);
  // get window width and height
  int width = 0, height = 0;
  glfwGetWindowSize(&width, &height);
  // create renderer
  Renderer *renderer = new DeferredRenderer(width, height);
  // create camera
  Camera *camera = new Camera();
  camera->setPosition(0, 170, 1000);
  camera->lookAt(0, 0, 0);
  camera->setAspectRatio(float(width) / float(height));
  // create root node
  Node *rootNode = new Node();
  rootNode->translate(glm::vec3(0.0f, 0.0f, 0.0f));
  rootNode->attachMesh(new PlaneMesh(glm::vec2(1000, 1000), glm::vec2(10, 10)));
 // create lightNode
  Light *directionalLight = new Light();
  directionalLight->setType(LT_DIRECTIONAL);
  directionalLight->setAmbientColor(glm::vec3(0, 0, 0));
  directionalLight->setDiffuseColor(glm::vec3(1, 1, 1));
  directionalLight->setSpecularColor(glm::vec3(1, 1, 1));
  directionalLight->setDirection(glm::normalize(glm::vec3(1, -1, -1)));
  rootNode->attachLight(directionalLight);
  // create child node
  Node *cubeNode = new Node();
  cubeNode->translate(glm::vec3(0.0f, 100.0f, 0.0f));
  cubeNode->attachMesh(new CubeMesh(glm::vec3(50.0f)));
  rootNode->attachNode(cubeNode);
  // start rendering
  double time = glfwGetTime();
  float timeDiff = 0;
  while (!glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED)) {
    // update time
    timeDiff = glfwGetTime() - time;
    // get time
    time += timeDiff;
    // toggle cull face
    if (glfwGetKey(GLFW_KEY_LCTRL))
      glDisable(GL_CULL_FACE);
    else
      glEnable(GL_CULL_FACE);
    // render one frame
    renderer->renderOneFrame(camera, rootNode);
    // swap front and back rendering buffers
    glfwSwapBuffers();
    // apply animations
    cubeNode->rotate(glm::vec3(0, 1, 0), timeDiff * 50);
  }
  // clean up
  glfwTerminate();
  
  return 0;
}
