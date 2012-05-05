#include "SubMesh.h"

#include "AxisAlignedBoundingBox.h"

#include <GL/glew.h>

namespace SimpleGL {
  class SubMeshPrivate {
  public:
    SubMeshPrivate() : vertexArray(0), vertexBuffer(0), indexBuffer(0), vertexCount(0), indexCount(0) {
    }

    ~SubMeshPrivate() {
    }

    GLuint vertexArray;
    GLuint vertexBuffer;
    GLuint indexBuffer;
    uint vertexCount;
    uint indexCount;
    AxisAlignedBoundingBox aabb;
    std::string materialName;
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

  bool SubMesh::setVertexData(uint vertexFormat, float vertexData[], uint vertexCount, uint stride) {
    // generate and bind vertex array object
    glBindVertexArray(d->vertexArray);
    // generate vertex buffer object
    glBindBuffer(GL_ARRAY_BUFFER, d->vertexBuffer);
    // assign data
    glBufferData(GL_ARRAY_BUFFER, stride * vertexCount, vertexData, GL_STATIC_DRAW);
    // define vertex data
    char *offset = 0;
    // vertex position
    if (vertexFormat & AT_POSITION) {
      glEnableVertexAttribArray(AT_POSITION);
      glVertexAttribPointer(AT_POSITION, 3, GL_FLOAT, GL_FALSE, stride, (void *)offset);
      // increase offset
      offset += 3 * sizeof(float);
    }
    // vertex normal
    if (vertexFormat & AT_NORMAL) {
      glEnableVertexAttribArray(AT_NORMAL);
      glVertexAttribPointer(AT_NORMAL, 3, GL_FLOAT, GL_FALSE, stride, (void *)offset);
      // increase offset
      offset += 3 * sizeof(float);
    }
    // vertex color
    if (vertexFormat & AT_COLOR) {
      glEnableVertexAttribArray(AT_COLOR);
      glVertexAttribPointer(AT_COLOR, 3, GL_FLOAT, GL_FALSE, stride, (void *)offset);
      // increase offset
      offset += 3 * sizeof(float);
    }
    // texture coordinate 0
    if (vertexFormat & AT_TEXCOORD0) {
      glEnableVertexAttribArray(AT_TEXCOORD0);
      glVertexAttribPointer(AT_TEXCOORD0, 2, GL_FLOAT, GL_FALSE, stride, (void *)offset);
      // increase offset
      offset += 2 * sizeof(float);
    }
    // texture coordinate 1
    if (vertexFormat & AT_TEXCOORD1) {
      glEnableVertexAttribArray(AT_TEXCOORD1);
      glVertexAttribPointer(AT_TEXCOORD1, 2, GL_FLOAT, GL_FALSE, stride, (void *)offset);
      // increase offset
      offset += 2 * sizeof(float);
    }
    // texture coordinate 2
    if (vertexFormat & AT_TEXCOORD2) {
      glEnableVertexAttribArray(AT_TEXCOORD2);
      glVertexAttribPointer(AT_TEXCOORD2, 2, GL_FLOAT, GL_FALSE, stride, (void *)offset);
      // increase offset
      offset += 2 * sizeof(float);
    }
    // unbind vertex array object
    glBindVertexArray(0);
    // update vertex count
    d->vertexCount = vertexCount;
    // return succes
    return true;
  }

  bool SubMesh::setIndexData(uint indexData[], uint indexCount) {
    // generate and bind vertex array object
    glBindVertexArray(d->vertexArray);
    // generate index buffer object
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, d->indexBuffer);
    // assign data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * indexCount, indexData, GL_STATIC_DRAW);
    // unbind vertex array object
    glBindVertexArray(0);
    // update index count
    d->indexCount = indexCount;
    // return succes
    return true;
  }

  AxisAlignedBoundingBox &SubMesh::aabb() const {
    return d->aabb;
  }

  void SubMesh::setMaterialName(const std::string &materialName) {
    d->materialName = materialName;
  }

  const std::string &SubMesh::materialName() const {
    return d->materialName;
  }

  bool SubMesh::render() const {
    glBindVertexArray(d->vertexArray);
    glDrawElements(GL_TRIANGLES, d->indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    // return success
    return true;
  }
}
