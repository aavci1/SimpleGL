#include "Texture.h"

#include <GL/glew.h>

#include <FreeImage.h>

namespace SimpleGL {
  class TexturePrivate {
  public:
    TexturePrivate() : id(0), width(0), height(0), bitsPerPixel(0), index(0) {
    }

    ~TexturePrivate() {
    }

    GLuint id;
    std::string path;
    uint width;
    uint height;
    uint bitsPerPixel;
    ushort index;
  };

  Texture::Texture(std::string path) : d(new TexturePrivate()) {
    // generate texture object
    glGenTextures(1, &d->id);
    // save path
    d->path = path;
  }

  Texture::~Texture() {
    // delete texture
    glDeleteTextures(1, &d->id);
    // delete data
    delete d;
  }

  std::string Texture::path() {
    return d->path;
  }

  bool Texture::load() {
    // check file header to find image format
    FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(d->path.c_str(), 0);
    // if cant find check extension
    if (fif == FIF_UNKNOWN)
      fif = FreeImage_GetFIFFromFilename(d->path.c_str());
    // if still unknown, return fail
    if (fif == FIF_UNKNOWN)
      return false;
    // if file type not supported, return fail
    if (!FreeImage_FIFSupportsReading(fif))
      return false;
    // load image
    FIBITMAP *dib = FreeImage_Load(fif, d->path.c_str());
    // if can't load, return fail
    if (!dib)
      return false;
    // get image data
    BYTE* imageData = FreeImage_GetBits(dib);
    // set file data
    d->width = FreeImage_GetWidth(dib);
    d->height = FreeImage_GetHeight(dib);
    d->bitsPerPixel = FreeImage_GetBPP(dib);
    // if anything failed, return fail
    if(imageData == NULL || d->width == 0 || d->height == 0)
      return false;
    // bind texture
    glBindTexture(GL_TEXTURE_2D, d->id);
    // fill in texture data
    if (d->bitsPerPixel == 32)
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, d->width, d->height, 0, GL_BGRA, GL_UNSIGNED_BYTE, imageData);
    else if (d->bitsPerPixel == 24)
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, d->width, d->height, 0, GL_BGR, GL_UNSIGNED_BYTE, imageData);
    else if (d->bitsPerPixel == 8)
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, d->width, d->height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, imageData);
    // generate mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);
    // unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
    // unload image
    FreeImage_Unload(dib);
    // return success
    return true;
  }

  bool Texture::select(ushort index) {
    // update index
    d->index = index;
    // activate texture
    glActiveTexture(GL_TEXTURE0 + d->index);
    glBindTexture(GL_TEXTURE_2D, d->id);
    // return success
    return true;
  }

  bool Texture::deselect() {
    // activate texture and unbind
    glActiveTexture(GL_TEXTURE0 + d->index);
    glBindTexture(GL_TEXTURE_2D, 0);
    // return success
    return true;
  }
}
