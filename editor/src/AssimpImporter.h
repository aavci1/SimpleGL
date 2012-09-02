#ifndef ASSIMPIMPORTER_H
#define ASSIMPIMPORTER_H

#include "Types.h"

namespace SimpleGL {
  class Mesh;
}

namespace AssimpImporter {
  SimpleGL::Mesh *import(const string &name, const string &path);
}

#endif // ASSIMPIMPORTER_H
