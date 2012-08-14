#include "Mesh.h"

#include <GL/glew.h>

namespace SimpleGL {
  class MeshPrivate {
  public:
    MeshPrivate() {
    }

    ~MeshPrivate() {
    }

    String name { "" };
    GLuint vertexArray { 0 };
    GLuint vertexBuffer { 0 };
    GLuint indexBuffer { 0 };
    uint vertexCount { 0 };
    uint indexCount { 0 };
  };

  Mesh::Mesh(const String &name) : d(new MeshPrivate()) {
    d->name = name;
    // generate vertex array
    glGenVertexArrays(1, &d->vertexArray);
    // generate vertex buffer
    glGenBuffers(1, &d->vertexBuffer);
    // generate index buffer
    glGenBuffers(1, &d->indexBuffer);
  }

  Mesh::~Mesh() {
    // delete vertex array
    glDeleteVertexArrays(1, &d->vertexArray);
    // delete vertex buffer
    glDeleteBuffers(1, &d->vertexBuffer);
    // delete index buffer
    glDeleteBuffers(1, &d->indexBuffer);
    // delete data
    delete d;
  }

  const String &Mesh::name() const {
    return d->name;
  }

  const bool Mesh::setVertexData(uint vertexFormat, float vertexData[], uint vertexCount, uint stride) {
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
    // bone info 0
    if (vertexFormat & AT_BONEINFO0) {
      // bone id
      glEnableVertexAttribArray(AP_BONEID0);
      glVertexAttribIPointer(AP_BONEID0, 1, GL_INT, stride, (void *)offset);
      // increase offset
      offset += sizeof(int);
      // bone weight
      glEnableVertexAttribArray(AP_BONEWEIGHT0);
      glVertexAttribPointer(AP_BONEWEIGHT0, 1, GL_FLOAT, GL_FALSE, stride, (void *)offset);
      // increase offset
      offset += sizeof(false);
    }
    // bone info 1
    if (vertexFormat & AT_BONEINFO1) {
      // bone id
      glEnableVertexAttribArray(AP_BONEID1);
      glVertexAttribIPointer(AP_BONEID1, 1, GL_INT, stride, (void *)offset);
      // increase offset
      offset += sizeof(int);
      // bone weight
      glEnableVertexAttribArray(AP_BONEWEIGHT1);
      glVertexAttribPointer(AP_BONEWEIGHT1, 1, GL_FLOAT, GL_FALSE, stride, (void *)offset);
      // increase offset
      offset += sizeof(false);
    }
    // bone info 2
    if (vertexFormat & AT_BONEINFO2) {
      // bone id
      glEnableVertexAttribArray(AP_BONEID2);
      glVertexAttribIPointer(AP_BONEID2, 1, GL_INT, stride, (void *)offset);
      // increase offset
      offset += sizeof(int);
      // bone weight
      glEnableVertexAttribArray(AP_BONEWEIGHT2);
      glVertexAttribPointer(AP_BONEWEIGHT2, 1, GL_FLOAT, GL_FALSE, stride, (void *)offset);
      // increase offset
      offset += sizeof(false);
    }
    // bone info 3
    if (vertexFormat & AT_BONEINFO3) {
      // bone id
      glEnableVertexAttribArray(AP_BONEID3);
      glVertexAttribIPointer(AP_BONEID3, 1, GL_INT, stride, (void *)offset);
      // increase offset
      offset += sizeof(int);
      // bone weight
      glEnableVertexAttribArray(AP_BONEWEIGHT3);
      glVertexAttribPointer(AP_BONEWEIGHT3, 1, GL_FLOAT, GL_FALSE, stride, (void *)offset);
      // increase offset
      offset += sizeof(false);
    }
    // unbind vertex array object
    glBindVertexArray(0);
    // update vertex count
    d->vertexCount = vertexCount;
    // return succes
    return true;
  }

  const bool Mesh::setIndexData(uint indexData[], uint indexCount) {
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

  void Mesh::render(Camera *camera) {
    // bind vertex array
    glBindVertexArray(d->vertexArray);
    // draw triangles
    glDrawElements(GL_TRIANGLES, d->indexCount, GL_UNSIGNED_INT, 0);
    // unbind vertex array
    glBindVertexArray(0);
  }
}
