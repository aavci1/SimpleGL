#include "ShaderProgram.h"

#include "FragmentShader.h"
#include "VertexShader.h"

#include <GL/glew.h>

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
    std::vector<AbstractShader *> shaders;
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

  uint ShaderProgram::id() {
    return d->id;
  }

  void ShaderProgram::addShader(AbstractShader *shader) {
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

  bool ShaderProgram::setUniform(std::string name, float value) {
    // get uniform location
    GLint location = glGetUniformLocation(d->id, name.c_str());
    if (location == -1)
      return false;
    // select this program
    glUseProgram(d->id);
    // set uniform value
    glUniform1f(location, value);
    // unselect this program
    glUseProgram(0);
  }
}
