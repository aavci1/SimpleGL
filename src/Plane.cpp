#include "Plane.h"

#include "Attribute.h"
#include "SubMesh.h"

namespace SimpleGL {
  Plane::Plane(glm::vec2 size, glm::vec2 uvTile) : Mesh() {
    float vertices[] = {
      -1.0f * size.x, 0.0f, +1.0f * size.y, 0.0f, 1.0f, 0.0f, -1.0f * uvTile.x, -1.0f * uvTile.y,
      +1.0f * size.x, 0.0f, +1.0f * size.y, 0.0f, 1.0f, 0.0f, +1.0f * uvTile.x, -1.0f * uvTile.y,
      +1.0f * size.x, 0.0f, -1.0f * size.y, 0.0f, 1.0f, 0.0f, +1.0f * uvTile.x, +1.0f * uvTile.y,
      -1.0f * size.x, 0.0f, -1.0f * size.y, 0.0f, 1.0f, 0.0f, -1.0f * uvTile.x, +1.0f * uvTile.y
    };

    uint indices[] = { 0, 1, 2, 0, 2, 3 };

    SubMesh *subMesh = createSubMesh();
    subMesh->setVertexData(SGL_POSITION | SGL_NORMAL | SGL_TEXCOORD0, vertices, 4, 32);
    subMesh->setIndexData(indices, 6);
  }
}
