#include "Material.h"

#include "Program.h"
#include "Root.h"

#include <GL/glew.h>

#include <FreeImage.h>

namespace SimpleGL {
  class MaterialPrivate {
  public:
    MaterialPrivate() {
    }

    ~MaterialPrivate() {
    }

    string name { "" };
    string program { "" };
    vector<GLuint> textures;
    CullFace cullFace { CF_BACK };
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

  void Material::addTexture(const string &path) {
    GLuint id = 0;
    // generate texture object
    glGenTextures(1, &id);
    // check file header to find image format
    FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(path.c_str(), 0);
    // if cant find check extension
    if (fif == FIF_UNKNOWN)
      fif = FreeImage_GetFIFFromFilename(path.c_str());
    // if still unknown, return fail
    if (fif == FIF_UNKNOWN)
      return;
    // if file type not supported, return fail
    if (!FreeImage_FIFSupportsReading(fif))
      return;
    // load image
    FIBITMAP *dib = FreeImage_Load(fif, path.c_str());
    // if can't load, return fail
    if (!dib)
      return;
    // get image data
    BYTE *imageData = FreeImage_GetBits(dib);
    // set file data
    uint width = FreeImage_GetWidth(dib);
    uint height = FreeImage_GetHeight(dib);
    uint bitsPerPixel = FreeImage_GetBPP(dib);
    // if anything failed, return fail
    if (imageData == NULL || width == 0 || height == 0)
      return;
    // bind texture
    glBindTexture(GL_TEXTURE_2D, id);
    // fill in texture data
    if (bitsPerPixel == 32)
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, imageData);
    else if (bitsPerPixel == 24)
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, imageData);
    else if (bitsPerPixel == 8)
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, imageData);
    // generate mipmaps
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    // unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
    // unload image
    FreeImage_Unload(dib);
    // add texture to the list
    d->textures.push_back(id);
  }

  const CullFace Material::cullFace() const {
    return d->cullFace;
  }

  void Material::setCullFace(const CullFace cullFace) {
    d->cullFace = cullFace;
  }

  void Material::bind() const {
    // set culling method
    if (d->cullFace == CF_NONE) {
      glDisable(GL_CULL_FACE);
    } else {
      glEnable(GL_CULL_FACE);
      // set face to cull
      if (d->cullFace == CF_BACK)
        glCullFace(GL_BACK);
      else
        glCullFace(GL_FRONT);
    }
    // bind program
    ProgramPtr program = Root::instance()->retrieveProgram(d->program);
    if (!program)
      return;
    program->bind();
    // bind textures
    for (uint j = 0; j < d->textures.size(); ++j) {
      // bind texture
      glActiveTexture(GL_TEXTURE0 + j);
      glBindTexture(GL_TEXTURE_2D, d->textures.at(j));
      // set uniform
      char buffer[8];
      snprintf(buffer, 8, "texture%d", j);
      program->setUniform(buffer, j);
    }
    // set specular parameters
    program->setUniform("specularIntensity", 1.0f);
    program->setUniform("specularPower", 64.0f);
  }
}
