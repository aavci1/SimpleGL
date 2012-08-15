#include "SubMesh.h"

#include <GL/glew.h>

namespace SimpleGL {
  class SubMeshPrivate {
  public:
    SubMeshPrivate() {
    }

    ~SubMeshPrivate() {
    }

    GLuint vertexArray { 0 };
    GLuint vertexBuffer { 0 };
    GLuint indexBuffer { 0 };
    uint vertexCount { 0 };
    uint indexCount { 0 };
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

  const bool SubMesh::setVertexData(uint vertexFormat, float vertexData[], uint vertexCount, uint stride) {
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
      glEnableVertexAttribArray(AP_POSITION);
      glVertexAttribPointer(AP_POSITION, 3, GL_FLOAT, GL_FALSE, stride, (void *)offset);
      // increase offset
      offset += 3 * sizeof(float);
    }
    // vertex normal
    if (vertexFormat & AT_NORMAL) {
      glEnableVertexAttribArray(AP_NORMAL);
      glVertexAttribPointer(AP_NORMAL, 3, GL_FLOAT, GL_FALSE, stride, (void *)offset);
      // increase offset
      offset += 3 * sizeof(float);
    }
    // vertex tangent/bitangent
    if (vertexFormat & AT_TANGENT_AND_BITANGENT) {
      glEnableVertexAttribArray(AP_TANGENT);
      glVertexAttribPointer(AP_TANGENT, 3, GL_FLOAT, GL_FALSE, stride, (void *)offset);
      // increase offset
      offset += 3 * sizeof(float);
      glEnableVertexAttribArray(AP_BITANGENT);
      glVertexAttribPointer(AP_BITANGENT, 3, GL_FLOAT, GL_FALSE, stride, (void *)offset);
      // increase offset
      offset += 3 * sizeof(float);
    }
    // vertex color
    if (vertexFormat & AT_COLOR) {
      glEnableVertexAttribArray(AP_COLOR);
      glVertexAttribPointer(AP_COLOR, 3, GL_FLOAT, GL_FALSE, stride, (void *)offset);
      // increase offset
      offset += 3 * sizeof(float);
    }
    // texture coordinate 0
    if (vertexFormat & AT_TEXCOORD0) {
      glEnableVertexAttribArray(AP_TEXCOORD0);
      glVertexAttribPointer(AP_TEXCOORD0, 2, GL_FLOAT, GL_FALSE, stride, (void *)offset);
      // increase offset
      offset += 2 * sizeof(float);
    }
    // texture coordinate 1
    if (vertexFormat & AT_TEXCOORD1) {
      glEnableVertexAttribArray(AP_TEXCOORD1);
      glVertexAttribPointer(AP_TEXCOORD1, 2, GL_FLOAT, GL_FALSE, stride, (void *)offset);
      // increase offset
      offset += 2 * sizeof(float);
    }
    // texture coordinate 2
    if (vertexFormat & AT_TEXCOORD2) {
      glEnableVertexAttribArray(AP_TEXCOORD2);
      glVertexAttribPointer(AP_TEXCOORD2, 2, GL_FLOAT, GL_FALSE, stride, (void *)offset);
      // increase offset
      offset += 2 * sizeof(float);
    }
    // texture coordinate 3
    if (vertexFormat & AT_TEXCOORD3) {
      glEnableVertexAttribArray(AP_TEXCOORD3);
      glVertexAttribPointer(AP_TEXCOORD3, 2, GL_FLOAT, GL_FALSE, stride, (void *)offset);
      // increase offset
      offset += 2 * sizeof(float);
    }
    // bone info
    if (vertexFormat & AT_BONES) {
      // bone ids
      glEnableVertexAttribArray(AP_BONEIDS);
      glVertexAttribIPointer(AP_BONEIDS, 4, GL_INT, stride, (void *)offset);
      // increase offset
      offset += 4 * sizeof(int);
      // bone weights
      glEnableVertexAttribArray(AP_BONEWEIGHTS);
      glVertexAttribPointer(AP_BONEWEIGHTS, 4, GL_FLOAT, GL_FALSE, stride, (void *)offset);
      // increase offset
      offset += 4 * sizeof(false);
    }
    // unbind vertex array object
    glBindVertexArray(0);
    // update vertex count
    d->vertexCount = vertexCount;
    // return succes
    return true;
  }

  const bool SubMesh::setIndexData(uint indexData[], uint indexCount) {
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

  void SubMesh::render(Camera *camera) {
    // bind vertex array
    glBindVertexArray(d->vertexArray);
    // draw triangles
    glDrawElements(GL_TRIANGLES, d->indexCount, GL_UNSIGNED_INT, 0);
    // unbind vertex array
    glBindVertexArray(0);
  }
}
