#ifndef ASSIMPIMPORTER_H
#define ASSIMPIMPORTER_H

#include "Types.h"

namespace SimpleGL {
  class Mesh;

  class AssimpImporterPrivate;
  class AssimpImporter {
  public:
    AssimpImporter();
    ~AssimpImporter();

    Mesh *loadMesh(const String &name, const String &path);

  private:
    AssimpImporterPrivate *d;
  };
}
#endif // ASSIMPIMPORTER_H
