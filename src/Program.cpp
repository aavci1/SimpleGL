#include "Program.h"

#include <GL/glew.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

namespace SimpleGL {
  class ProgramPrivate {
  public:
    ProgramPrivate() {
    }

    ~ProgramPrivate() {
    }

    String name { "" };
    String log { "" };
    GLuint id { 0 };
    std::vector<GLuint> shaders;
  };

  Program::Program(const String &name) : d(new ProgramPrivate()) {
    d->name = name;
    // create program object
    d->id = glCreateProgram();
  }

  Program::~Program() {
    // delete shaders
    for (uint i = 0; i < d->shaders.size(); ++i)
      glDeleteShader(d->shaders[i]);
    // delete program
    glDeleteProgram(d->id);
    // delete data
    delete d;
  }

  const String &Program::name() const {
    return d->name;
  }

  const bool Program::loadShaderFromPath(ShaderType type, const String &path) {
    // read file
    std::ifstream in(path.c_str());
    std::stringstream buffer;
    buffer << in.rdbuf();
    // load shader from source
    return loadShaderFromSource(type, String(buffer.str()));
  }

  const bool Program::loadShaderFromSource(ShaderType type, const String &source) {
    GLenum shaderType;
    if (type == ST_VERTEX)
      shaderType = GL_VERTEX_SHADER;
    else if (type == ST_GEOMETRY)
      shaderType = GL_GEOMETRY_SHADER;
    else
      shaderType = GL_FRAGMENT_SHADER;
    // create shader object
    GLuint id = glCreateShader(shaderType);
    // set shader source
    const char *s = strdup(source.c_str());
    glShaderSource(id, 1, &s, NULL);
    delete s;
    // clear error message
    d->log = "";
    // compile shader
    glCompileShader(id);
    // check compile status
    GLint status;
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);
    // if compilation failed
    if (status == GL_FALSE) {
      // get error message length
      GLint messageLength;
      glGetShaderiv(id, GL_INFO_LOG_LENGTH, &messageLength);
      // create message buffer
      GLchar *message = new GLchar[messageLength + 1];
      glGetShaderInfoLog(id, messageLength, NULL, message);
      // update message
      d->log = message;
      // clean up
      delete[] message;
      // delete shader object
      glDeleteShader(id);
      // return fail
      return false;
    }
    // add shader object to the list
    d->shaders.push_back(id);
    // return success
    return true;
  }

  const bool Program::link() {
    // attach shaders
    for (uint i = 0; i < d->shaders.size(); ++i)
      glAttachShader(d->id, d->shaders[i]);
    // bind generic vertex attributes
    glBindAttribLocation(d->id, AP_POSITION, "POSITION");
    glBindAttribLocation(d->id, AP_NORMAL, "NORMAL");
    glBindAttribLocation(d->id, AP_COLOR, "COLOR");
    glBindAttribLocation(d->id, AP_TEXCOORD0, "TEXCOORD0");
    glBindAttribLocation(d->id, AP_TEXCOORD1, "TEXCOORD1");
    glBindAttribLocation(d->id, AP_TEXCOORD2, "TEXCOORD2");
    glBindAttribLocation(d->id, AP_TEXCOORD3, "TEXCOORD3");
    glBindAttribLocation(d->id, AP_BONEIDS, "BONEIDS");
    glBindAttribLocation(d->id, AP_BONEWEIGHTS, "BONEWEIGHTS");
    // link the program
    glLinkProgram(d->id);
    // check link status
    GLint status;
    glGetProgramiv(d->id, GL_LINK_STATUS, &status);
    // if failed
    if (status == GL_FALSE) {
      // get error message length
      GLint messageLength;
      glGetProgramiv(d->id, GL_INFO_LOG_LENGTH, &messageLength);
      // create message buffer
      GLchar *message = new GLchar[messageLength + 1];
      glGetProgramInfoLog(d->id, messageLength, NULL, message);
      // update message
      d->log = message;
      // clean up
      delete[] message;
      // return fail
      return false;
    }
    // detach shaders
    for (uint i = 0; i < d->shaders.size(); ++i)
      glDetachShader(d->id, d->shaders[i]);
    // return success
    return true;
  }

  const String &Program::log() const {
    return d->log;
  }

  const bool Program::setUniform(const String &name, uint value) const {
    // get uniform location
    GLint location = glGetUniformLocation(d->id, name.c_str());
    if (location == -1)
      return false;
    // set uniform value
    glUniform1i(location, value);
    // return succes
    return true;
  }

  const bool Program::setUniform(const String &name, int value) const {
    // get uniform location
    GLint location = glGetUniformLocation(d->id, name.c_str());
    if (location == -1)
      return false;
    // set uniform value
    glUniform1i(location, value);
    // return succes
    return true;
  }

  const bool Program::setUniform(const String &name, float value) const {
    // get uniform location
    GLint location = glGetUniformLocation(d->id, name.c_str());
    if (location == -1)
      return false;
    // set uniform value
    glUniform1f(location, value);
    // return succes
    return true;
  }

  const bool Program::setUniform(const String &name, const Vector2f &value) const {
    // get uniform location
    GLint location = glGetUniformLocation(d->id, name.c_str());
    if (location == -1)
      return false;
    // set uniform value
    glUniform2f(location, value.x, value.y);
    // return succes
    return true;
  }

  const bool Program::setUniform(const String &name, const Vector3f &value) const {
    // get uniform location
    GLint location = glGetUniformLocation(d->id, name.c_str());
    if (location == -1)
      return false;
    // set uniform value
    glUniform3f(location, value.x, value.y, value.z);
    // return succes
    return true;
  }

  const bool Program::setUniform(const String &name, const Matrix4f &value) const {
    // get uniform location
    GLint location = glGetUniformLocation(d->id, name.c_str());
    if (location == -1)
      return false;
    // set uniform value
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    // return succes
    return true;
  }

  const bool Program::bind() const {
    // select shader
    glUseProgram(d->id);
    // return succes
    return true;
  }

  const bool Program::unbind() const {
    // deselect shader
    glUseProgram(0);
    // return succes
    return true;
  }
}
