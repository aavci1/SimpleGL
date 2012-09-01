#include "InputStream.h"

#include <fstream>

namespace SimpleGL {
  class InputStreamPrivate {
  public:
    InputStreamPrivate() {
    }

    fstream *stream { nullptr };
  };

  InputStream::InputStream(const string &path) : d(new InputStreamPrivate()) {
      d->stream = new fstream(path, ios::binary | ios::in);
  }

  InputStream::~InputStream() {
    if (d->stream) {
      d->stream->close();
      delete d->stream;
    }
    delete d;
  }

  void InputStream::read(char *buffer, uint32_t numBytes) {
    d->stream->read(buffer, numBytes);
  }

  InputStream &InputStream::operator >> (char &c) {
    d->stream->read((char *)&c, sizeof(c));
    return *this;
  }

  InputStream &InputStream::operator >> (uint8_t &u) {
    d->stream->read((char *)&u, sizeof(u));
    return *this;
  }

  InputStream &InputStream::operator >> (uint16_t &u) {
    d->stream->read((char *)&u, sizeof(u));
    return *this;
  }

  InputStream &InputStream::operator >> (uint32_t &u) {
    d->stream->read((char *)&u, sizeof(u));
    return *this;
  }

  InputStream &InputStream::operator >> (float &f) {
    d->stream->read((char *)&f, sizeof(f));
    return *this;
  }

  InputStream &InputStream::operator >> (double &f) {
    d->stream->read((char *)&f, sizeof(f));
    return *this;
  }

  InputStream &InputStream::operator >> (string &s) {
    // read a zero-terminated string, maximum 255 bytes length
    // last byte is left to make sure string ends with zero
    uint8_t buffer[256] = { 0 };
    for (int k = 0; k < 255; ++k) {
      (*this) >> buffer[k];
      if (buffer[k] == 0)
        break;
    }
    // create string
    s = string((char *)buffer);
    // return this
    return *this;
  }

  InputStream &InputStream::operator >> (Matrix4f &m) {
    d->stream->read((char *)&m, sizeof(m));
    return *this;
  }
}
