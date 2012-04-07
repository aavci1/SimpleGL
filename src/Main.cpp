#include <fstream>
#include <iostream>
#include <sstream>

#include <GL/glew.h>
#include <GL/glfw.h>

#include "FragmentShader.h"
#include "VertexShader.h"
#include "ShaderProgram.h"

using namespace SimpleGL;

std::string readAll(const char *fileName) {
  std::ifstream in(fileName);
  std::stringstream buffer;
  buffer << in.rdbuf();
  
  return std::string(buffer.str());
}

void GLFWCALL resize(int width, int height) {
  //   printf("new size: %dx%d\n", width, height);
  glViewport(0, 0, (GLsizei) width, (GLsizei) height);
}

int main(int argc, char **argv) {
  if (!glfwInit()) {
    printf("error: cannot initialize glfw.\n");
    return 1;
  }
  GLFWvidmode listModes[200];
  int nummodes = glfwGetVideoModes(listModes, 200);
  printf("Available modes: \n");
  for (int i = 0; i < nummodes; ++i)
    printf("%4dx%4d, %d bit colors\n", listModes[i].Width, listModes[i].Height, listModes[i].RedBits + listModes[i].GreenBits + listModes[i].BlueBits);
  GLFWvidmode desktopMode;
  glfwGetDesktopMode(&desktopMode);
  printf("%4dx%4d, %d bit colors\n", desktopMode.Width, desktopMode.Height, desktopMode.RedBits + desktopMode.GreenBits + desktopMode.BlueBits);
  // request a OpenGL 3.3+ context
  glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 4);
  glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
  // request opengl profile
  glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // request full screen anti aliasing
  // glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
  // request stereo
  // glfwOpenWindowHint(GLFW_STEREO, GL_TRUE);
  // try to open the window
  if (!glfwOpenWindow(1024, 768, 8, 8, 8, 0, 24, 0, GLFW_WINDOW)) {
    printf("error: cannot open glfw window.\n");
    glfwTerminate();
    return 1;
  }
  // set window resize call back, must be set after window creation
  glfwSetWindowSizeCallback(resize);
  // print opengl info
  printf("OpenGL vendor  : %s\n", glGetString(GL_VENDOR));
  printf("OpenGL renderer: %s\n", glGetString(GL_RENDERER));
  printf("OpenGL version : %s\n", glGetString(GL_VERSION));
  printf("GLSL version   : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
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

  float fTriangle[18] = {
    -0.5f, -0.3f, 0.0f,
    1.0f, 0.0f, 0.0f,
    +0.5f, -0.3f, 0.0f,
    0.0f, 1.0f, 0.0f,
    +0.0f, 0.7f, 0.0f,
    0.0f, 0.0f, 1.0f
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
  while (!glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED)) {
    // get time
    time = glfwGetTime();
    // clear color buffer
    glClear(GL_COLOR_BUFFER_BIT);
    // select shader to use
    glUseProgram(shaderProgram->id());
    // select vertex array object to draw
    glBindVertexArray(vao);
    // draw item
    glDrawArrays(GL_TRIANGLES, 0, 3);
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
