#include "SphereMesh.h"

#include "Attribute.h"

#include <math.h>

namespace SimpleGL {
  SphereMesh::SphereMesh(float r, int slices, int segments) : Mesh() {
    // allocate the vertex buffer
    uint vertexCount = (segments + 1) * (slices+1);
    float *vertices = new float[vertexCount * 8];
    uint indexCount = 6 * segments * (slices + 1);
    uint *indices = new uint[indexCount];
    // vertex pointer
    float *pVertex = vertices;
    // index pointer
    uint *pIndices = indices;
    // index of the vertice for current face
    uint wVerticeIndex = 0 ;
    // allocate index buffer
    float fDeltaRingAngle = (M_PI / segments);
    float fDeltaSegAngle = (2 * M_PI / slices);
    // Generate the group of rings for the sphere
    for( int ring = 0; ring <= segments; ring++ ) {
      float r0 = r * sinf (ring * fDeltaRingAngle);
      float y0 = r * cosf (ring * fDeltaRingAngle);
      // Generate the group of segments for the current ring
      for(int seg = 0; seg <= slices; seg++) {
        float x0 = r0 * sinf(seg * fDeltaSegAngle);
        float z0 = r0 * cosf(seg * fDeltaSegAngle);
        // add vertex position
        *pVertex++ = x0;
        *pVertex++ = y0;
        *pVertex++ = z0;
        // add vertex normal
        glm::vec3 vNormal = glm::normalize(glm::vec3(x0, y0, z0));
        *pVertex++ = vNormal.x;
        *pVertex++ = vNormal.y;
        *pVertex++ = vNormal.z;
        // add texture coordinate
        *pVertex++ = (float) seg / (float) slices * 2 + 1;
        *pVertex++ = (float) ring / (float) segments * 2 + 1;
        // assign indices
        if (ring != segments) {
          // each vertex (except the last) has six indices pointing to it
          *pIndices++ = wVerticeIndex + slices + 1;
          *pIndices++ = wVerticeIndex;
          *pIndices++ = wVerticeIndex + slices;
          *pIndices++ = wVerticeIndex + slices + 1;
          *pIndices++ = wVerticeIndex + 1;
          *pIndices++ = wVerticeIndex;
          // next vertice
          wVerticeIndex ++;
        }
      }
    }
    // set vertex and index data
    setVertexData(SGL_POSITION | SGL_NORMAL | SGL_TEXCOORD0, vertices, vertexCount, 32);
    setIndexData(indices, indexCount);
    // clean up
    delete[] vertices;
    delete[] indices;
  }
}
