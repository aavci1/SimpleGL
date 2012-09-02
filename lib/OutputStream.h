#ifndef OUTPUTSTREAM_H
#define OUTPUTSTREAM_H

#include "Types.h"

namespace SimpleGL {
  class OutputStreamPrivate;

  class OutputStream {
  public:
    OutputStream(const string &path);
    ~OutputStream();

    void write(const char *buffer, const uint32_t numBytes);

    OutputStream &operator << (const char u);
    OutputStream &operator << (const uint8_t u);
    OutputStream &operator << (const uint16_t u);
    OutputStream &operator << (const uint32_t u);
    OutputStream &operator << (const long l);
    OutputStream &operator << (const float f);
    OutputStream &operator << (const double f);
    OutputStream &operator << (const string s);
    OutputStream &operator << (const Vector3f v);
    OutputStream &operator << (const Quaternion q);
    OutputStream &operator << (const Matrix4f m);

  private:
    OutputStreamPrivate *d;
  };
}

#endif // OUTPUTSTREAM_H
