#include "Quad.h"

#include "Attribute.h"
#include "SubMesh.h"

namespace SimpleGL {
  Quad::Quad(glm::vec2 size) : Mesh() {
    float vertices[] = {
      -1.0f * size.x, -1.0f * size.y, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
       1.0f * size.x, -1.0f * size.y, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
       1.0f * size.x, +1.0f * size.y, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
      -1.0f * size.x, +1.0f * size.y, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f
    };

    uint indices[] = {
      0, 1, 2,
      0, 2, 3
    };

    SubMesh *subMesh = createSubMesh();
    subMesh->setVertexData(SGL_POSITION | SGL_NORMAL | SGL_TEXCOORD0, vertices, 24, 32);
    subMesh->setIndexData(indices, 36);
  }

  void Quad::render() {
    subMeshes().at(0)->render();
  }
}
