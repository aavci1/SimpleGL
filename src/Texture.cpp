#include "Texture.h"

#include <GL/glew.h>

#include <FreeImage.h>

namespace SimpleGL {
  class TexturePrivate {
  public:
    TexturePrivate() : id(0), sampler(0), width(0), height(0), bitsPerPixel(0) {
    }

    ~TexturePrivate() {
    }

    GLuint id;
    GLuint sampler;
    std::string path;
    uint width;
    uint height;
    uint bitsPerPixel;
  };

  Texture::Texture(std::string path) : d(new TexturePrivate()) {
    // generate texture object
    glGenTextures(1, &d->id);
    // generate sampler object
    glGenSamplers(1, &d->sampler);
    // save path
    d->path = path;
  }

  Texture::~Texture() {
    // delete texture
    glDeleteTextures(1, &d->id);
    // delete sampler
    glDeleteSamplers(1, &d->sampler);
    // delete data
    delete d;
  }

  uint Texture::id() {
    return d->id;
  }

  uint Texture::sampler() {
    return d->sampler;
  }

  uint Texture::width() {
    return d->width;
  }

  uint Texture::height() {
    return d->height;
  }

  uint Texture::bitsPerPixel() {
    return d->bitsPerPixel;
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
    int iFormat = d->bitsPerPixel == 24 ? GL_BGR : d->bitsPerPixel == 8 ? GL_LUMINANCE : 0;
    // fill in texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, d->width, d->height, 0, iFormat, GL_UNSIGNED_BYTE, imageData);
    // generate mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);
    // unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
    // unload image
    FreeImage_Unload(dib);

    return true;
  }
}
