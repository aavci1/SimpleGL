#include "ShaderProgram.h"

#include "Attribute.h"
#include "FragmentShader.h"
#include "VertexShader.h"

#include <GL/glew.h>

#include <glm/ext.hpp>

#include <vector>

namespace SimpleGL {
  class ShaderProgramPrivate {
  public:
    ShaderProgramPrivate() {
    }

    ~ShaderProgramPrivate() {
      for (int i = 0; i < shaders.size(); ++i)
        delete shaders[i];
    }

    GLuint id;
    std::string message;
    std::vector<Shader *> shaders;
  };

  ShaderProgram::ShaderProgram() : d(new ShaderProgramPrivate()) {
    d->id = glCreateProgram();
  }

  ShaderProgram::~ShaderProgram() {
    // delete program
    glDeleteProgram(d->id);
    // delete data
    delete d;
  }

  void ShaderProgram::addShader(Shader *shader) {
    d->shaders.push_back(shader);
  }

  bool ShaderProgram::compileAndLink() {
    for (int i = 0; i < d->shaders.size(); ++i) {
      if (!d->shaders[i]->compile()) {
        d->message = d->shaders[i]->message();
        return false;
      }
      glAttachShader(d->id, d->shaders[i]->id());
    }
    // bind generic vertex attributes
    glBindAttribLocation(d->id, SGL_POSITION, "sglPosition");
    glBindAttribLocation(d->id, SGL_NORMAL, "sglNormal");
    glBindAttribLocation(d->id, SGL_COLOR, "sglColor");
    glBindAttribLocation(d->id, SGL_TEXCOORD0, "sglTexCoord0");
    glBindAttribLocation(d->id, SGL_TEXCOORD1, "sglTexCoord1");
    glBindAttribLocation(d->id, SGL_TEXCOORD2, "sglTexCoord2");
    // link the program
    glLinkProgram(d->id);
    // check link status
    GLint status;
    glGetProgramiv (d->id, GL_LINK_STATUS, &status);
    // if failed
    if (status == GL_FALSE) {
      // get error message length
      GLint messageLength;
      glGetProgramiv(d->id, GL_INFO_LOG_LENGTH, &messageLength);
      // create message buffer
      GLchar *message = new GLchar[messageLength + 1];
      glGetProgramInfoLog(d->id, messageLength, NULL, message);
      // update message
      d->message = message;
      // clean up
      delete[] message;
      // return fail
      return false;
    }
    // detach all shaders
    for (int i = 0; i < d->shaders.size(); ++i)
      glDetachShader(d->id, d->shaders[i]->id());
    // return succes
    return true;
  }

  std::string ShaderProgram::message() {
    return d->message;
  }

  bool ShaderProgram::setUniform(std::string name, uint value) {
    // get uniform location
    GLint location = glGetUniformLocation(d->id, name.c_str());
    if (location == -1)
      return false;
    // set uniform value
    glUniform1i(location, value);
    // return succes
    return true;
  }

  bool ShaderProgram::setUniform(std::string name, int value) {
    // get uniform location
    GLint location = glGetUniformLocation(d->id, name.c_str());
    if (location == -1)
      return false;
    // set uniform value
    glUniform1i(location, value);
    // return succes
    return true;
  }

  bool ShaderProgram::setUniform(std::string name, float value) {
    // get uniform location
    GLint location = glGetUniformLocation(d->id, name.c_str());
    if (location == -1)
      return false;
    // set uniform value
    glUniform1f(location, value);
    // return succes
    return true;
  }


  bool ShaderProgram::setUniform(std::string name, const glm::vec2 &value) {
    // get uniform location
    GLint location = glGetUniformLocation(d->id, name.c_str());
    if (location == -1)
      return false;
    // set uniform value
    glUniform2f(location, value.x, value.y);
    // return succes
    return true;
  }

  bool ShaderProgram::setUniform(std::string name, const glm::vec3 &value) {
    // get uniform location
    GLint location = glGetUniformLocation(d->id, name.c_str());
    if (location == -1)
      return false;
    // set uniform value
    glUniform3f(location, value.x, value.y, value.z);
    // return succes
    return true;
  }

  bool ShaderProgram::setUniform(std::string name, const glm::mat4 &value) {
    // get uniform location
    GLint location = glGetUniformLocation(d->id, name.c_str());
    if (location == -1)
      return false;
    // set uniform value
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    // return succes
    return true;
  }

  bool ShaderProgram::select() const {
    // select shader
    glUseProgram(d->id);
    // return succes
    return true;
  }

  bool ShaderProgram::deselect() const {
    // deselect shader
    glUseProgram(0);
    // return succes
    return true;
  }
}
