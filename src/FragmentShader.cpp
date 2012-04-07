#include "FragmentShader.h"

#include <cstring>

#include <GL/glew.h>

namespace SimpleGL {
  class FragmentShaderPrivate {
  public:
    FragmentShaderPrivate() : id(0) {
    }

    ~FragmentShaderPrivate() {
    }

    GLuint id;
    std::string source;
    std::string message;
  };

  FragmentShader::FragmentShader(std::string source) : d(new FragmentShaderPrivate()) {
    // create shader object
    d->id = glCreateShader(GL_FRAGMENT_SHADER);
    // set shader source
    const char *s = strdup(source.c_str());
    glShaderSource(d->id, 1, &s, NULL);
    delete s;
    // save source
    d->source = source;
  }

  FragmentShader::~FragmentShader() {
    // delete shader
    glDeleteShader(d->id);
    // delete data
    delete d;
  }

  uint FragmentShader::id() {
    return d->id;
  }

  bool FragmentShader::compile() {
    // clear error message
    d->message = "";
    // compile shader
    glCompileShader(d->id);
    // check compile status
    GLint status;
    glGetShaderiv(d->id, GL_COMPILE_STATUS, &status);
    // if compilation failed
    if (status == GL_FALSE) {
      // get error message length
      GLint messageLength;
      glGetShaderiv(d->id, GL_INFO_LOG_LENGTH, &messageLength);
      // create message buffer
      GLchar *message = new GLchar[messageLength + 1];
      glGetShaderInfoLog(d->id, messageLength, NULL, message);
      // update message
      d->message = message;
      // clean up
      delete[] message;
      // return fail
      return false;
    }
    return true;
  }

  std::string FragmentShader::message() {
    return d->message;
  }
}
