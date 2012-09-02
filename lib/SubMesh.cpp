#include "SubMesh.h"

#include <GL/glew.h>

namespace SimpleGL {
  class SubMeshPrivate {
  public:
    SubMeshPrivate() {
    }

    ~SubMeshPrivate() {
      delete vertexData;
      delete indexData;
    }

    GLuint vertexArray { 0 };
    GLuint vertexBuffer { 0 };
    GLuint indexBuffer { 0 };

    uint16_t vertexFormat { 0 };
    uint16_t vertexSize { 0 };
    uint32_t vertexCount { 0 };
    float *vertexData { 0 };

    uint16_t indexSize { 4 };
    uint16_t indexCount { 0 };
    uint32_t *indexData { 0 };
  };

  SubMesh::SubMesh() : d(new SubMeshPrivate()) {
    // generate vertex array
    glGenVertexArrays(1, &d->vertexArray);
    // generate vertex buffer
    glGenBuffers(1, &d->vertexBuffer);
    // generate index buffer
    glGenBuffers(1, &d->indexBuffer);
  }

  SubMesh::~SubMesh() {
    // delete vertex array
    glDeleteVertexArrays(1, &d->vertexArray);
    // delete vertex buffer
    glDeleteBuffers(1, &d->vertexBuffer);
    // delete index buffer
    glDeleteBuffers(1, &d->indexBuffer);
    // delete data
    delete d;
  }

  const uint16_t SubMesh::vertexFormat() const {
    return d->vertexFormat;
  }

  const uint16_t SubMesh::vertexSize() const {
    return d->vertexSize;
  }

  const uint32_t SubMesh::vertexCount() const {
    return d->vertexCount;
  }

  const char *SubMesh::vertexData() const {
    return (char *)d->vertexData;
  }

  const uint16_t SubMesh::indexSize() const {
    return d->indexSize;
  }

  const uint32_t SubMesh::indexCount() const {
    return d->indexCount;
  }

  const char *SubMesh::indexData() const {
    return (char *)d->indexData;
  }

  const bool SubMesh::setVertexData(float *vertexData, uint vertexCount, uint vertexFormat) {
    // calculate vertex size
    d->vertexSize = 0;
    vector<pair<AttributeType, int>> attributes = {
      { AT_POSITION, 3 * sizeof(float) },
      { AT_NORMAL, 3 * sizeof(float) },
      { AT_TANGENT_AND_BITANGENT, 6 * sizeof(float) },
      { AT_COLOR, 3 * sizeof(float) },
      { AT_TEXCOORD0, 2 * sizeof(float) },
      { AT_TEXCOORD1, 2 * sizeof(float) },
      { AT_TEXCOORD2, 2 * sizeof(float) },
      { AT_TEXCOORD3, 2 * sizeof(float) },
      { AT_BONES, 4 * sizeof(int) + 4 * sizeof(float) }
    };
    for (auto attribute: attributes)
      if (vertexFormat & attribute.first)
        d->vertexSize += attribute.second;
    // generate and bind vertex array object
    glBindVertexArray(d->vertexArray);
    // generate vertex buffer object
    glBindBuffer(GL_ARRAY_BUFFER, d->vertexBuffer);
    // assign data
    glBufferData(GL_ARRAY_BUFFER, d->vertexSize * vertexCount, vertexData, GL_STATIC_DRAW);
    // define vertex data
    char *offset = 0;
    // vertex position
    if (vertexFormat & AT_POSITION) {
      glEnableVertexAttribArray(AP_POSITION);
      glVertexAttribPointer(AP_POSITION, 3, GL_FLOAT, GL_FALSE, d->vertexSize, (void *)offset);
      // increase offset
      offset += 3 * sizeof(float);
    }
    // vertex normal
    if (vertexFormat & AT_NORMAL) {
      glEnableVertexAttribArray(AP_NORMAL);
      glVertexAttribPointer(AP_NORMAL, 3, GL_FLOAT, GL_FALSE, d->vertexSize, (void *)offset);
      // increase offset
      offset += 3 * sizeof(float);
    }
    // vertex tangent/bitangent
    if (vertexFormat & AT_TANGENT_AND_BITANGENT) {
      glEnableVertexAttribArray(AP_TANGENT);
      glVertexAttribPointer(AP_TANGENT, 3, GL_FLOAT, GL_FALSE, d->vertexSize, (void *)offset);
      // increase offset
      offset += 3 * sizeof(float);
      glEnableVertexAttribArray(AP_BITANGENT);
      glVertexAttribPointer(AP_BITANGENT, 3, GL_FLOAT, GL_FALSE, d->vertexSize, (void *)offset);
      // increase offset
      offset += 3 * sizeof(float);
    }
    // vertex color
    if (vertexFormat & AT_COLOR) {
      glEnableVertexAttribArray(AP_COLOR);
      glVertexAttribPointer(AP_COLOR, 3, GL_FLOAT, GL_FALSE, d->vertexSize, (void *)offset);
      // increase offset
      offset += 3 * sizeof(float);
    }
    // texture coordinate 0
    if (vertexFormat & AT_TEXCOORD0) {
      glEnableVertexAttribArray(AP_TEXCOORD0);
      glVertexAttribPointer(AP_TEXCOORD0, 2, GL_FLOAT, GL_FALSE, d->vertexSize, (void *)offset);
      // increase offset
      offset += 2 * sizeof(float);
    }
    // texture coordinate 1
    if (vertexFormat & AT_TEXCOORD1) {
      glEnableVertexAttribArray(AP_TEXCOORD1);
      glVertexAttribPointer(AP_TEXCOORD1, 2, GL_FLOAT, GL_FALSE, d->vertexSize, (void *)offset);
      // increase offset
      offset += 2 * sizeof(float);
    }
    // texture coordinate 2
    if (vertexFormat & AT_TEXCOORD2) {
      glEnableVertexAttribArray(AP_TEXCOORD2);
      glVertexAttribPointer(AP_TEXCOORD2, 2, GL_FLOAT, GL_FALSE, d->vertexSize, (void *)offset);
      // increase offset
      offset += 2 * sizeof(float);
    }
    // texture coordinate 3
    if (vertexFormat & AT_TEXCOORD3) {
      glEnableVertexAttribArray(AP_TEXCOORD3);
      glVertexAttribPointer(AP_TEXCOORD3, 2, GL_FLOAT, GL_FALSE, d->vertexSize, (void *)offset);
      // increase offset
      offset += 2 * sizeof(float);
    }
    // bone info
    if (vertexFormat & AT_BONES) {
      // bone ids
      glEnableVertexAttribArray(AP_BONEIDS);
      glVertexAttribIPointer(AP_BONEIDS, 4, GL_INT, d->vertexSize, (void *)offset);
      // increase offset
      offset += 4 * sizeof(int);
      // bone weights
      glEnableVertexAttribArray(AP_BONEWEIGHTS);
      glVertexAttribPointer(AP_BONEWEIGHTS, 4, GL_FLOAT, GL_FALSE, d->vertexSize, (void *)offset);
      // increase offset
      offset += 4 * sizeof(false);
    }
    // unbind vertex array object
    glBindVertexArray(0);
    // copy of the buffer
    d->vertexData = (float *)(new uint8_t[d->vertexSize * vertexCount]);
    memcpy(d->vertexData, vertexData, d->vertexSize * vertexCount);
    d->vertexCount = vertexCount;
    d->vertexFormat = vertexFormat;
    // return succes
    return true;
  }

  const bool SubMesh::setIndexData(uint32_t *indexData, uint indexCount) {
    // generate and bind vertex array object
    glBindVertexArray(d->vertexArray);
    // generate index buffer object
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, d->indexBuffer);
    // assign data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * indexCount, indexData, GL_STATIC_DRAW);
    // unbind vertex array object
    glBindVertexArray(0);
    // copy the buffer
    d->indexData = new uint32_t[indexCount];
    memcpy(d->indexData, indexData, sizeof(uint32_t) * indexCount);
    d->indexCount = indexCount;
    // return succes
    return true;
  }

  void SubMesh::render(Camera *camera) {
    // bind vertex array
    glBindVertexArray(d->vertexArray);
    // draw triangles
    glDrawElements(GL_TRIANGLES, d->indexCount, GL_UNSIGNED_INT, 0);
    // unbind vertex array
    glBindVertexArray(0);
  }
}
