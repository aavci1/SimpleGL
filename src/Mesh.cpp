#include "Mesh.h"

#include <GL/glew.h>

namespace SimpleGL {
  class MeshPrivate {
  public:
    MeshPrivate(const String &name) : name(name), vertexArray(0), vertexBuffer(0), indexBuffer(0), vertexCount(0), indexCount(0) {
    }

    ~MeshPrivate() {
    }

    String name;
    GLuint vertexArray;
    GLuint vertexBuffer;
    GLuint indexBuffer;
    uint vertexCount;
    uint indexCount;
  };

  Mesh::Mesh(const String &name) : d(new MeshPrivate(name)) {
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
    // texture coordinate 4
    if (vertexFormat & AT_TEXCOORD4) {
      glEnableVertexAttribArray(AP_TEXCOORD4);
      glVertexAttribPointer(AP_TEXCOORD4, 2, GL_FLOAT, GL_FALSE, stride, (void *)offset);
      // increase offset
      offset += 2 * sizeof(float);
    }
    // texture coordinate 5
    if (vertexFormat & AT_TEXCOORD5) {
      glEnableVertexAttribArray(AP_TEXCOORD5);
      glVertexAttribPointer(AP_TEXCOORD5, 2, GL_FLOAT, GL_FALSE, stride, (void *)offset);
      // increase offset
      offset += 2 * sizeof(float);
    }
    // texture coordinate 6
    if (vertexFormat & AT_TEXCOORD6) {
      glEnableVertexAttribArray(AP_TEXCOORD6);
      glVertexAttribPointer(AP_TEXCOORD6, 2, GL_FLOAT, GL_FALSE, stride, (void *)offset);
      // increase offset
      offset += 2 * sizeof(float);
    }
    // texture coordinate 7
    if (vertexFormat & AT_TEXCOORD7) {
      glEnableVertexAttribArray(AP_TEXCOORD7);
      glVertexAttribPointer(AP_TEXCOORD7, 2, GL_FLOAT, GL_FALSE, stride, (void *)offset);
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
