#ifndef FBXIMPORTER_H
#define FBXIMPORTER_H

#include "Types.h"


namespace FBXImporter {
    SimpleGL::ModelPtr import(const string &name, const string &path);
}

#endif // FBXIMPORTER_H
