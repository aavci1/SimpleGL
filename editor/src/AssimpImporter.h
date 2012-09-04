#ifndef ASSIMPIMPORTER_H
#define ASSIMPIMPORTER_H

#include "Types.h"

namespace AssimpImporter {
  SimpleGL::ModelPtr import(const string &name, const string &path);
}

#endif // ASSIMPIMPORTER_H
