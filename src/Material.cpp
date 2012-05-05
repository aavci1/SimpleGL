#include "Material.h"

#include "Program.h"
#include "Texture.h"
#include "Util.h"

#include <vector>

namespace SimpleGL {
  class MaterialPrivate {
  public:
    MaterialPrivate(const std::string &name) : name(name), program(0) {
    }
    ~MaterialPrivate() {
      // delete textures
      for (uint i = 0; i < textures.size(); ++i)
        delete textures.at(i);
      // delete program
      delete program;
    }

    std::string name;
    Program *program;
    std::vector<Texture *> textures;
  };

  Material::Material(const std::string &name) : d(new MaterialPrivate(name)) {
  }

  Material::~Material() {
    delete d;
  }

  const std::string &Material::name() const {
    return d->name;
  }

  bool Material::setProgram(const std::string &vertexShaderPath, const std::string &fragmentShaderPath) {
    if (d->program) {
      // delete if a program exists before
      delete d->program;
      d->program = 0;
    }
    // load program
    d->program = new Program("");
    d->program->loadShaderFromPath(ST_VERTEX, vertexShaderPath);
    d->program->loadShaderFromPath(ST_FRAGMENT, fragmentShaderPath);
    // try to compile and link the program
    if (!d->program->link()) {
      std::cerr << d->program->errorMessage() << std::endl;
      // delete the program
      delete d->program;
      d->program = 0;
      // return fail
      return false;
    }
    // return success
    return true;
  }

  Program *Material::program() const {
    return d->program;
  }

  bool Material::addTexture(const std::string &texturePath) {
    Texture *texture = new Texture("", texturePath);
    // add texture to the list
    d->textures.push_back(texture);
    // return success
    return true;
  }

  void Material::select() {
    // select textures
    for (uint i = 0; i < d->textures.size(); ++i)
      d->textures.at(i)->bind(i);
    // select program
    d->program->bind();
    // set samplers
    for (uint i = 0; i < d->textures.size(); ++i)
      d->program->setUniform("texture" + Util::toString(i), i);
    // set specular parameters
    d->program->setUniform("sglSpecularIntensity", 1.0f);
    d->program->setUniform("sglSpecularPower", 64.0f);
  }

  void Material::deselect() {
    // deselect program
    d->program->unbind();
    // deselect textures
    for (uint i = 0; i < d->textures.size(); ++i)
      d->textures.at(i)->unbind();
  }
}
