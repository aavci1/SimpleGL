#include "Material.h"

#include "Program.h"
#include "Root.h"
#include "Texture.h"

#include <sstream>

namespace SimpleGL {
  class MaterialPrivate {
  public:
    MaterialPrivate(const String &name) : name(name), program("") {
    }

    ~MaterialPrivate() {
    }

    String name;
    String program;
    std::vector<String> textures;
  };

  Material::Material(const String &name) : d(new MaterialPrivate(name)) {
  }

  Material::~Material() {
    delete d;
  }

  const String &Material::name() const {
    return d->name;
  }

  const String &Material::program() const {
    return d->program;
  }

  void Material::setProgram(const String &program) {
    d->program = program;
  }

  const std::vector<String> &Material::textures() const {
    return d->textures;
  }

  void Material::addTexture(const String &texture) {
    d->textures.push_back(texture);
  }

  const std::string toString(const int number) {
    std::stringstream ss;
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
      program->setUniform("texture" + toString(j), j);
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
