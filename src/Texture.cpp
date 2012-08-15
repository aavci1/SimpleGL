#include "Texture.h"

#include <GL/glew.h>

#include <FreeImage.h>

namespace SimpleGL {
  class TexturePrivate {
  public:
    TexturePrivate() {
    }

    ~TexturePrivate() {
    }

    string name { "" };
    string path { "" };
    GLuint id { 0 };
    GLuint index { 0 };
  };

  Texture::Texture(const string &name, const string &path) : d(new TexturePrivate()) {
    d->name = name;
    d->path = path;
    // generate texture object
    glGenTextures(1, &d->id);
    // check file header to find image format
    FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(d->path.c_str(), 0);
    // if cant find check extension
    if (fif == FIF_UNKNOWN)
      fif = FreeImage_GetFIFFromFilename(d->path.c_str());
    // if still unknown, return fail
    if (fif == FIF_UNKNOWN)
      return;
    // if file type not supported, return fail
    if (!FreeImage_FIFSupportsReading(fif))
      return;
    // load image
    FIBITMAP *dib = FreeImage_Load(fif, d->path.c_str());
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
    glBindTexture(GL_TEXTURE_2D, d->id);
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
  }

  Texture::~Texture() {
    // delete texture
    glDeleteTextures(1, &d->id);
    // delete data
    delete d;
  }

  const string &Texture::name() const {
    return d->name;
  }

  const string &Texture::path() const {
    return d->path;
  }

  const bool Texture::bind(uint index) const {
    // update index
    d->index = index;
    // activate texture
    glActiveTexture(GL_TEXTURE0 + d->index);
    glBindTexture(GL_TEXTURE_2D, d->id);
    // return success
    return true;
  }

  const bool Texture::unbind() const {
    // activate texture and unbind
    glActiveTexture(GL_TEXTURE0 + d->index);
    glBindTexture(GL_TEXTURE_2D, 0);
    // return success
    return true;
  }
}
