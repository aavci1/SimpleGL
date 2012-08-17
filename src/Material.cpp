#include "Material.h"

#include "Program.h"
#include "Root.h"
#include "Texture.h"

#include <sstream>

namespace SimpleGL {
  class MaterialPrivate {
  public:
    MaterialPrivate() {
    }

    ~MaterialPrivate() {
    }

    string name { "" };
    string program { "" };
    vector<string> textures;
  };

  Material::Material(const string &name) : d(new MaterialPrivate()) {
    d->name = name;
  }

  Material::~Material() {
    delete d;
  }

  const string &Material::name() const {
    return d->name;
  }

  const string &Material::program() const {
    return d->program;
  }

  void Material::setProgram(const string &program) {
    d->program = program;
  }

  const vector<string> &Material::textures() const {
    return d->textures;
  }

  void Material::addTexture(const string &texture) {
    d->textures.push_back(texture);
  }

  const string tostring(const int number) {
    stringstream ss;
    ss << number;
    return ss.str();
  }

  void Material::bind() {
    // bind program
    Program *program = Root::instance()->retrieveProgram(d->program);
    if (!program)
      return;
    program->bind();
    // bind textures
    for (uint j = 0; j < d->textures.size(); ++j) {
      Texture *texture = Root::instance()->retrieveTexture(d->textures.at(j));
      if (!texture)
        continue;
      texture->bind(j);
      program->setUniform("texture" + tostring(j), j);
    }
    // set specular parameters
    program->setUniform("specularIntensity", 1.0f);
    program->setUniform("specularPower", 64.0f);
  }

  void Material::unbind() {
    // unbind textures
    for (uint j = 0; j < d->textures.size(); ++j) {
      Texture *texture = Root::instance()->retrieveTexture(d->textures.at(j));
      if (!texture)
        continue;
      texture->unbind();
    }
    // unbind program
    Program *program = Root::instance()->retrieveProgram(d->program);
    if (program)
      program->unbind();
  }
}
