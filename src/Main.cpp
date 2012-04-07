#include "FragmentShader.h"
#include "VertexShader.h"
#include "ShaderProgram.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <GL/glew.h>
#include <GL/glfw.h>

#include <fstream>
#include <iostream>
#include <sstream>

using namespace SimpleGL;

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
  projMatrix = glm::perspectiveFov(90.0f, 1.0f * width, 1.0f * height, 10.0f, 10000.0f);
}

int main(int argc, char **argv) {
  if (!glfwInit()) {
    printf("error: cannot initialize glfw.\n");
    return 1;
  }
  // request opengl version
  glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 4);
  glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
  // request opengl profile
  glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // try to open the window
  if (!glfwOpenWindow(1024, 768, 8, 8, 8, 0, 24, 0, GLFW_WINDOW)) {
    printf("error: cannot open glfw window.\n");
    glfwTerminate();
    return 1;
  }
  // set window resize call back, must be set after window creation
  glfwSetWindowSizeCallback(resize);
  // initialize glew, must be after window creation
  if (glewInit() != GLEW_OK) {
    printf("error: cannot initialize glew.\n");
    return 1;
  }
  ShaderProgram *shaderProgram = new ShaderProgram();
  shaderProgram->addShader(new VertexShader(readAll("media/projection.vert")));
  shaderProgram->addShader(new FragmentShader(readAll("media/minimal.frag")));
  if (!shaderProgram->compileAndLink())
    printf("Compiling shader program failed:\n%s", shaderProgram->message().c_str());

  float fTriangle[] = {
    -5.0f, -5.0f, +5.0f, 1.0f, 0.0f, 0.0f,
     0.0f, +5.0f,  0.0f, 0.0f, 1.0f, 0.0f,
    -5.0f, -5.0f, -5.0f, 0.0f, 0.0f, 1.0f,

    +5.0f, -5.0f, +5.0f, 1.0f, 0.0f, 0.0f,
     0.0f, +5.0f,  0.0f, 0.0f, 1.0f, 0.0f,
    -5.0f, -5.0f, +5.0f, 0.0f, 0.0f, 1.0f,

    +5.0f, -5.0f, +5.0f, 1.0f, 0.0f, 0.0f,
     0.0f, +5.0f,  0.0f, 0.0f, 0.0f, 1.0f,
    +5.0f, -5.0f, -5.0f, 1.0f, 0.0f, 0.0f,

    +5.0f, -5.0f, +5.0f, 1.0f, 0.0f, 0.0f,
     0.0f, +5.0f,  0.0f, 0.0f, 0.0f, 1.0f,
    -5.0f, -5.0f, +5.0f, 1.0f, 0.0f, 0.0f,
  };
  // generate and bind vertex array object
  uint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  // generate vertex buffer object
  uint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  // assign data
  glBufferData(GL_ARRAY_BUFFER, sizeof(fTriangle), fTriangle, GL_STATIC_DRAW);
  // enable position
  glEnableVertexAttribArray(SGL_POSITION);
  glVertexAttribPointer(SGL_POSITION, 3, GL_FLOAT, GL_FALSE, 24, 0);
  // enable color
  glEnableVertexAttribArray(SGL_COLOR);
  glVertexAttribPointer(SGL_COLOR, 3, GL_FLOAT, GL_FALSE, 24, (void *)12);
  // unbind vertex buffer object
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  // unbind vertex array object
  glBindVertexArray(0);
  // while not escape pressed and window is not closed
  double time = glfwGetTime();
  // set color to clear background
  glClearColor(0.0f, 0.5f, 1.0f, 1.0f);
  modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 0));
  viewMatrix = glm::translate(viewMatrix, glm::vec3(0, 0, -20));
  while (!glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED)) {
    // get time
    time = glfwGetTime();
    // update matrices
    modelMatrix = glm::rotate(modelMatrix, 0.01f, glm::vec3(0, 1, 0));
    viewMatrix = glm::rotate(viewMatrix, 0.01f, glm::vec3(1, 0, 0));
    // clear color buffer
    glClear(GL_COLOR_BUFFER_BIT);
    // update uniforms
    shaderProgram->setUniform("sglWorldViewProjMatrix", projMatrix * viewMatrix * modelMatrix);
    // select shader to use
    glUseProgram(shaderProgram->id());
    // select vertex array object to draw
    glBindVertexArray(vao);
    // draw item
    glDrawArrays(GL_TRIANGLES, 0, 12);
    // unselect shader
    glUseProgram(0);
    // swap front and back rendering buffers
    glfwSwapBuffers();
  }
  // clean up
  delete shaderProgram;
  // clean up
  glfwTerminate();
  
  return 0;
}
