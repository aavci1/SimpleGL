#include "Program.h"

#include <GL/glew.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <vector>

namespace SimpleGL {
  class ProgramPrivate {
  public:
    ProgramPrivate() {
    }

    ~ProgramPrivate() {
    }

    string name { "" };
    string log { "" };
    GLuint id { 0 };
    vector<GLuint> shaders;
    vector<string> uniforms;
    map<string, GLint> uniformLocations;
  };

  Program::Program(const string &name) : d(new ProgramPrivate()) {
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

  const string &Program::name() const {
    return d->name;
  }

  const bool Program::loadShaderFromPath(ShaderType type, const string &path) {
    // read file
    ifstream in(path.c_str());
    // check file
    if (!in.is_open())
      return false;
    // read file contents
    stringstream buffer;
    buffer << in.rdbuf();
    // load shader from source
    return loadShaderFromSource(type, string(buffer.str()));
  }

  const bool Program::loadShaderFromSource(ShaderType type, const string &source) {
    GLenum shaderType;
    if (type == ST_VERTEX)
      shaderType = GL_VERTEX_SHADER;
    else if (type == ST_GEOMETRY)
      shaderType = GL_GEOMETRY_SHADER;
    else
      shaderType = GL_FRAGMENT_SHADER;
    // create shader object
    GLuint id = glCreateShader(shaderType);
    if (id == 0)
      return false;
    // set shader source
    const char *s = source.c_str();
    glShaderSource(id, 1, &s, NULL);
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
      GLint messageLength = 0;
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

  void Program::addUniform(const string &name) {
    d->uniforms.push_back(name);
  }

  const bool Program::link() {
    // attach shaders
    for (uint i = 0; i < d->shaders.size(); ++i)
      glAttachShader(d->id, d->shaders[i]);
    // bind generic vertex attributes
    glBindAttribLocation(d->id, AP_POSITION, "POSITION");
    glBindAttribLocation(d->id, AP_NORMAL, "NORMAL");
    glBindAttribLocation(d->id, AP_TANGENT, "TANGENT");
    glBindAttribLocation(d->id, AP_BITANGENT, "BITANGENT");
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
    // resolve uniform locations
    for (const string &uniform: d->uniforms)
      d->uniformLocations[uniform] = glGetUniformLocation(d->id, uniform.c_str());
    // return success
    return true;
  }

  const string &Program::log() const {
    return d->log;
  }

  void Program::setUniform(const string &name, uint value) const {
    glUniform1i(d->uniformLocations[name], value);
  }

  void Program::setUniform(const string &name, int value) const {
    glUniform1i(d->uniformLocations[name], value);
  }

  void Program::setUniform(const string &name, float value) const {
    glUniform1f(d->uniformLocations[name], value);
  }

  void Program::setUniform(const string &name, const Vector2f &value) const {
    glUniform2f(d->uniformLocations[name], value.x, value.y);
  }

  void Program::setUniform(const string &name, const Vector3f &value) const {
    glUniform3f(d->uniformLocations[name], value.x, value.y, value.z);
  }

  void Program::setUniform(const string &name, const Matrix4f &value) const {
    glUniformMatrix4fv(d->uniformLocations[name], 1, GL_FALSE, glm::value_ptr(value));
  }

  void Program::setUniform4fv(const string &name, const int numItems, const float *value) const {
    glUniformMatrix4fv(d->uniformLocations[name], numItems, GL_FALSE, value);
  }

  void Program::bind() const {
    glUseProgram(d->id);
  }
}
