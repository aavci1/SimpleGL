#ifndef INPUTSTREAM_H
#define INPUTSTREAM_H

#include "Types.h"

namespace SimpleGL {
  class InputStreamPrivate;

  class InputStream {
  public:
    InputStream(const string &path);
    ~InputStream();

    void read(char *buffer, uint32_t numBytes);

    InputStream &operator >> (char &c);
    InputStream &operator >> (uint8_t &u);
    InputStream &operator >> (uint16_t &u);
    InputStream &operator >> (uint32_t &u);
    InputStream &operator >> (long &l);
    InputStream &operator >> (float &f);
    InputStream &operator >> (double &f);
    InputStream &operator >> (string &s);
    InputStream &operator >> (Vector3f &v);
    InputStream &operator >> (Quaternion &q);
    InputStream &operator >> (Matrix4f &m);

  private:
    InputStreamPrivate *d;
  };
}

#endif // INPUTSTREAM_H
