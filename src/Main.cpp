#include "Attribute.h"
#include "FragmentShader.h"
#include "VertexShader.h"
#include "ShaderProgram.h"
#include "SubMesh.h"
#include "Texture.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <GL/glew.h>
#include <GL/glfw.h>

#include <fstream>
#include <iostream>
#include <sstream>

using namespace SimpleGL;

float vertices[] = {
  -7.5f, -5.0f, +7.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  +7.5f, -5.0f, +7.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
  +7.5f, -5.0f, -7.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
  -7.5f, -5.0f, -7.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,

  -7.5f, +5.0f, +7.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
  +7.5f, +5.0f, +7.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
  +7.5f, +5.0f, -7.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
  -7.5f, +5.0f, -7.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f
};
uint indices[] = {
  // bottom
  0, 3, 2,
  0, 2, 1,
  // top
  4, 5, 6,
  4, 6, 7,
  // back
  2, 3, 7,
  2, 7, 6,
  // left
  3, 0, 4,
  3, 4, 7,
  // right
  1, 2, 6,
  1, 6, 5,
  // front
  0, 1, 5,
  0, 5, 4
};

glm::mat4 projMatrix;
glm::mat4 viewMatrix;
glm::mat4 modelMatrix;

std::string readAll(const char *fileName) {
  std::ifstream in(fileName);
  std::stringstream buffer;
  buffer << in.rdbuf();
  
  return std::string(buffer.str());
}

void GLFWCALL resize(int width, int height) {
  //   printf("new size: %dx%d\n", width, height);
  glViewport(0, 0, (GLsizei) width, (GLsizei) height);
  // update perspective
  projMatrix = glm::perspectiveFov(60.0f, 1.0f * width, 1.0f * height, 1.0f, 10000.0f);
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
  // load texture
  Texture *texture = new Texture("media/laminate.jpg");
  if (!texture->load())
    printf("error: can not load texture %s.", texture->path().c_str());
  // load shader
  ShaderProgram *shaderProgram = new ShaderProgram();
  shaderProgram->addShader(new VertexShader(readAll("media/textured_vp.glsl")));
  shaderProgram->addShader(new FragmentShader(readAll("media/textured_fp.glsl")));
  if (!shaderProgram->compileAndLink())
    printf("error: can not compile shader:\n%s", shaderProgram->message().c_str());
  // load submesh
  SubMesh *submesh = new SubMesh();
  submesh->setVertexData(SGL_POSITION | SGL_TEXCOORD0 | SGL_COLOR, vertices, 8, 32);
  submesh->setIndexData(indices, 36);
  // while not escape pressed and window is not closed
  double time = glfwGetTime();
  float timeDiff = 0;
  // set color to clear background
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glEnable(GL_CULL_FACE);
  glEnable(GL_TEXTURE_2D);
  modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 0));
  viewMatrix = glm::translate(viewMatrix, glm::vec3(0, 0, -25));
  // start rendering
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
    // apply animations
    if (!glfwGetKey(GLFW_KEY_SPACE))
      modelMatrix = glm::rotate(modelMatrix, timeDiff * 50, glm::vec3(0, 1, 0));
    // clear color buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // select texture
    texture->select(1);
    // update uniforms
    shaderProgram->setUniform("sglModelViewProjMatrix", projMatrix * viewMatrix * modelMatrix);
    shaderProgram->setUniform("sglSampler", 1);
    // select shader
    shaderProgram->select();
    // draw submesh
    submesh->draw();
    // deselect shader
    shaderProgram->deselect();
    // deselect texture
    texture->deselect();
    // swap front and back rendering buffers
    glfwSwapBuffers();
  }
  // clean up
  delete submesh;
  delete texture;
  delete shaderProgram;
  // clean up
  glfwTerminate();
  
  return 0;
}
