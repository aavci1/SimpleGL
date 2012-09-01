#include "OutputStream.h"

#include <fstream>

namespace SimpleGL {
  class OutputStreamPrivate {
  public:
    OutputStreamPrivate() {
    }

    fstream *stream { nullptr };
  };

  OutputStream::OutputStream(const string &path) : d(new OutputStreamPrivate()) {
      d->stream = new fstream(path, ios::binary | ios::out | ios::trunc);
  }

  OutputStream::~OutputStream() {
    if (d->stream) {
      d->stream->close();
      delete d->stream;
    }
    delete d;
  }

  void OutputStream::write(const char *buffer, const uint32_t numBytes) {
    d->stream->write(buffer, numBytes);
  }

  OutputStream &OutputStream::operator << (const char c) {
    d->stream->write((char *)&c, sizeof(c));
    return *this;
  }

  OutputStream &OutputStream::operator << (const uint8_t u) {
    d->stream->write((char *)&u, sizeof(u));
    return *this;
  }

  OutputStream &OutputStream::operator << (const uint16_t u) {
    d->stream->write((char *)&u, sizeof(u));
    return *this;
  }

  OutputStream &OutputStream::operator << (const uint32_t u) {
    d->stream->write((char *)&u, sizeof(u));
    return *this;
  }

  OutputStream &OutputStream::operator << (const float f) {
    d->stream->write((char *)&f, sizeof(f));
    return *this;
  }

  OutputStream &OutputStream::operator << (const double f) {
    d->stream->write((char *)&f, sizeof(f));
    return *this;
  }

  OutputStream &OutputStream::operator << (const string s) {
    d->stream->write(s.c_str(), s.length() + 1);
    return *this;
  }
}