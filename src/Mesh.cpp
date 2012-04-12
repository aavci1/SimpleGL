#include "Mesh.h"

#include "Attribute.h"

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
  };

  Mesh::Mesh() : d(new SubMeshPrivate()) {
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

  bool Mesh::setVertexData(uint vertexFormat, float vertexData[], uint vertexCount, uint stride) {
    // generate and bind vertex array object
    glBindVertexArray(d->vertexArray);
    // generate vertex buffer object
    glBindBuffer(GL_ARRAY_BUFFER, d->vertexBuffer);
    // assign data
    glBufferData(GL_ARRAY_BUFFER, stride * vertexCount, vertexData, GL_STATIC_DRAW);
    // define vertex data
    uint offset = 0;
    // vertex position
    if (vertexFormat & SGL_POSITION) {
      glEnableVertexAttribArray(SGL_POSITION);
      glVertexAttribPointer(SGL_POSITION, 3, GL_FLOAT, GL_FALSE, stride, (void *)offset);
      // increase offset
      offset += 3 * sizeof(float);
    }
    // vertex normal
    if (vertexFormat & SGL_NORMAL) {
      glEnableVertexAttribArray(SGL_NORMAL);
      glVertexAttribPointer(SGL_NORMAL, 3, GL_FLOAT, GL_FALSE, stride, (void *)offset);
      // increase offset
      offset += 3 * sizeof(float);
    }
    // vertex color
    if (vertexFormat & SGL_COLOR) {
      glEnableVertexAttribArray(SGL_COLOR);
      glVertexAttribPointer(SGL_COLOR, 3, GL_FLOAT, GL_FALSE, stride, (void *)offset);
      // increase offset
      offset += 3 * sizeof(float);
    }
    // texture coordinate 0
    if (vertexFormat & SGL_TEXCOORD0) {
      glEnableVertexAttribArray(SGL_TEXCOORD0);
      glVertexAttribPointer(SGL_TEXCOORD0, 2, GL_FLOAT, GL_FALSE, stride, (void *)offset);
      // increase offset
      offset += 2 * sizeof(float);
    }
    // texture coordinate 1
    if (vertexFormat & SGL_TEXCOORD1) {
      glEnableVertexAttribArray(SGL_TEXCOORD1);
      glVertexAttribPointer(SGL_TEXCOORD1, 2, GL_FLOAT, GL_FALSE, stride, (void *)offset);
      // increase offset
      offset += 2 * sizeof(float);
    }
    // texture coordinate 2
    if (vertexFormat & SGL_TEXCOORD2) {
      glEnableVertexAttribArray(SGL_TEXCOORD2);
      glVertexAttribPointer(SGL_TEXCOORD2, 2, GL_FLOAT, GL_FALSE, stride, (void *)offset);
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

  bool Mesh::setIndexData(uint indexData[], uint indexCount) {
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

  bool Mesh::render() const {
    glBindVertexArray(d->vertexArray);
    glDrawElements(GL_TRIANGLES, d->indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    // return success
    return true;
  }
}
