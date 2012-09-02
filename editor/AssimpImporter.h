#ifndef ASSIMPIMPORTER_H
#define ASSIMPIMPORTER_H

#include "Types.h"

namespace SimpleGL {
  class Mesh;
}

class AssimpImporterPrivate;
class AssimpImporter {
public:
  AssimpImporter();
  ~AssimpImporter();

  SimpleGL::Mesh *import(const string &name, const string &path);

private:
  AssimpImporterPrivate *d;
};

#endif // ASSIMPIMPORTER_H
