#include "MeshManager.h"

#include "Attribute.h"
#include "AxisAlignedBoundingBox.h"
#include "Material.h"
#include "MaterialManager.h"
#include "Mesh.h"
#include "SubMesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <map>

#include <stdio.h>

namespace SimpleGL {
  static MeshManager *_instance = 0;

  class MeshManagerPrivate {
  public:
    MeshManagerPrivate() : importer(new Assimp::Importer()) {
    }

    ~MeshManagerPrivate() {
      delete importer;
    }

    void createQuad(SubMesh *subMesh, float width, float height) {
      float vertices[] = {
        -1.0f * width, -1.0f * height, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
         1.0f * width, -1.0f * height, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
         1.0f * width, +1.0f * height, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -1.0f * width, +1.0f * height, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f
      };

      uint indices[] = {
        0, 1, 2,
        0, 2, 3
      };
      // set vertex and index data
      subMesh->setVertexData(SGL_POSITION | SGL_NORMAL | SGL_TEXCOORD0, vertices, 24, 32);
      subMesh->setIndexData(indices, 36);
    }

    void createPlane(SubMesh *subMesh, float width, float height, float uTile, float vTile) {
      float vertices[] = {
        -1.0f * width, 0.0f, +1.0f * height, 0.0f, 1.0f, 0.0f, -1.0f * uTile, -1.0f * vTile,
        +1.0f * width, 0.0f, +1.0f * height, 0.0f, 1.0f, 0.0f, +1.0f * uTile, -1.0f * vTile,
        +1.0f * width, 0.0f, -1.0f * height, 0.0f, 1.0f, 0.0f, +1.0f * uTile, +1.0f * vTile,
        -1.0f * width, 0.0f, -1.0f * height, 0.0f, 1.0f, 0.0f, -1.0f * uTile, +1.0f * vTile
      };
      uint indices[] = { 0, 1, 2, 0, 2, 3 };
      // set vertex and index data
      subMesh->setVertexData(SGL_POSITION | SGL_NORMAL | SGL_TEXCOORD0, vertices, 4, 32);
      subMesh->setIndexData(indices, 6);
    }

    void createCube(SubMesh *subMesh, float width, float height, float depth) {
      float vertices[] = {
        -1.0f * width, -1.0f * height,  1.0f * depth, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f,
         1.0f * width, -1.0f * height,  1.0f * depth, 0.0f, 0.0f, +1.0f, 1.0f, 0.0f,
         1.0f * width,  1.0f * height,  1.0f * depth, 0.0f, 0.0f, +1.0f, 1.0f, 1.0f,
        -1.0f * width,  1.0f * height,  1.0f * depth, 0.0f, 0.0f, +1.0f, 0.0f, 1.0f,

        -1.0f * width, -1.0f * height, -1.0f * depth, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        -1.0f * width,  1.0f * height, -1.0f * depth, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
         1.0f * width,  1.0f * height, -1.0f * depth, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
         1.0f * width, -1.0f * height, -1.0f * depth, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        -1.0f * width,  1.0f * height, -1.0f * depth, 0.0f, +1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f * width,  1.0f * height,  1.0f * depth, 0.0f, +1.0f, 0.0f, 0.0f, 0.0f,
         1.0f * width,  1.0f * height,  1.0f * depth, 0.0f, +1.0f, 0.0f, 1.0f, 0.0f,
         1.0f * width,  1.0f * height, -1.0f * depth, 0.0f, +1.0f, 0.0f, 1.0f, 1.0f,

        -1.0f * width, -1.0f * height, -1.0f * depth, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
         1.0f * width, -1.0f * height, -1.0f * depth, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
         1.0f * width, -1.0f * height,  1.0f * depth, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -1.0f * width, -1.0f * height,  1.0f * depth, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,

         1.0f * width, -1.0f * height, -1.0f * depth, +1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
         1.0f * width,  1.0f * height, -1.0f * depth, +1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
         1.0f * width,  1.0f * height,  1.0f * depth, +1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
         1.0f * width, -1.0f * height,  1.0f * depth, +1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

        -1.0f * width, -1.0f * height, -1.0f * depth, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -1.0f * width, -1.0f * height,  1.0f * depth, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -1.0f * width,  1.0f * height,  1.0f * depth, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -1.0f * width,  1.0f * height, -1.0f * depth, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f
      };

      uint indices[] = {
        0, 1, 2,
        0, 2, 3,

        4, 5, 6,
        4, 6, 7,

        8, 9, 10,
        8, 10, 11,

        12, 13, 14,
        12, 14, 15,

        16, 17, 18,
        16, 18, 19,

        20, 21, 22,
        20, 22, 23
      };
      // set vertex and index data
      subMesh->setVertexData(SGL_POSITION | SGL_NORMAL | SGL_TEXCOORD0, vertices, 24, 32);
      subMesh->setIndexData(indices, 36);
    }

    void createSphere(SubMesh *subMesh, float radius, uint slices, uint stacks) {
      // allocate the vertex buffer
      uint vertexCount = (stacks + 1) * (slices+1);
      float *vertices = new float[vertexCount * 8];
      uint indexCount = 6 * stacks * (slices + 1);
      uint *indices = new uint[indexCount];
      // vertex pointer
      float *pVertex = vertices;
      // index pointer
      uint *pIndices = indices;
      // index of the vertice for current face
      uint wVerticeIndex = 0 ;
      // allocate index buffer
      float fDeltaRingAngle = (M_PI / stacks);
      float fDeltaSegAngle = (2 * M_PI / slices);
      // Generate the group of rings for the sphere
      for( int ring = 0; ring <= stacks; ring++ ) {
        float r0 = radius * sinf (ring * fDeltaRingAngle);
        float y0 = radius * cosf (ring * fDeltaRingAngle);
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
          *pVertex++ = (float) ring / (float) stacks * 2 + 1;
          // assign indices
          if (ring != stacks) {
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
      subMesh->setVertexData(SGL_POSITION | SGL_NORMAL | SGL_TEXCOORD0, vertices, vertexCount, 32);
      subMesh->setIndexData(indices, indexCount);
      // clean up
      delete[] vertices;
      delete[] indices;
    }

    Assimp::Importer *importer;
  };

  MeshManager::MeshManager() : d(new MeshManagerPrivate()) {
  }

  MeshManager::~MeshManager() {
    delete d;
  }

  MeshManager *MeshManager::instance() {
    if (!_instance)
      _instance = new MeshManager();
    // return instance
    return _instance;
  }

  Mesh *MeshManager::createQuad(float width, float height) {
    Mesh *mesh = new Mesh();
    // create a submesh
    SubMesh *subMesh = mesh->createSubMesh();
    // fill the sub mesh
    d->createQuad(subMesh, width, height);
    // return mesh
    return mesh;
  }

  Mesh *MeshManager::createPlane(float width, float height, float uTile, float vTile) {
    Mesh *mesh = new Mesh();
    // create a submesh
    SubMesh *subMesh = mesh->createSubMesh();
    // fill the sub mesh
    d->createPlane(subMesh, width, height, uTile, vTile);
    // return mesh
    return mesh;
  }

  Mesh *MeshManager::createCube(float width, float height, float depth) {
    Mesh *mesh = new Mesh();
    // create a submesh
    SubMesh *subMesh = mesh->createSubMesh();
    // fill the sub mesh
    d->createCube(subMesh, width, height, depth);
    // return mesh
    return mesh;
  }

  Mesh *MeshManager::createSphere(float radius, uint slices, uint stacks) {
    Mesh *mesh = new Mesh();
    // create a submesh
    SubMesh *subMesh = mesh->createSubMesh();
    // fill the sub mesh
    d->createSphere(subMesh, radius, slices, stacks);
    // return mesh
    return mesh;
  }

  Mesh *MeshManager::loadMesh(const std::string &path) {
    Mesh *mesh = new Mesh();
    // try loading the mesh using assimp
    const aiScene *scene = d->importer->ReadFile(path.c_str(), aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_PreTransformVertices | aiProcess_FixInfacingNormals);
    // return mesh if scene cannot be loaded
    if (!scene)
      return mesh;
    // material names
    std::map<int, std::string> materialNames;
    // load materials
    if (scene->HasMaterials()) {
      // get directory of the mesh
      std::string dir = path.substr(0, path.find_last_of("/"));
      // Initialize the materials
      for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
        const aiMaterial* material = scene->mMaterials[i];
        // check diffuse texture
        if (material->GetTextureCount(aiTextureType_DIFFUSE) <= 0)
          continue;
        // get texture path
        aiString texturePath;
        if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) != AI_SUCCESS)
          continue;
        // remove directory part
        std::string s(texturePath.data);
        int index = s.find_last_of("/");
        if (index != -1)
          s = s.substr(index + 1);
        // make up texture path
        std::string texturePathString = dir + "/" + s;
        // create material
        Material *mat = MaterialManager::instance()->createMaterial(texturePathString);
        // add texture
        mat->addTexture(texturePathString);
        // add program
        mat->setProgram("media/deferred_geometry_vp.glsl", "media/deferred_geometry_fp.glsl");
        // set material index
        materialNames[i] = texturePathString;
      }
    }
    // load meshes
    if (scene->HasMeshes()) {
      for (int i = 0; i < scene->mNumMeshes; ++i) {
        const struct aiMesh *aimesh = scene->mMeshes[i];
        // primitive contains primitives other than triangles skip
        // should be use together with aiProcess_SortByPType
        if (aimesh->mPrimitiveTypes & (aiPrimitiveType_POINT | aiPrimitiveType_LINE | aiPrimitiveType_POLYGON))
          continue;
        // find which attributes exist and calculate stride size
        uint attributes = 0;
        uint stride = 0;
        if (aimesh->HasPositions()) {
          stride += 3;
          attributes |= SGL_POSITION;
        }
        if (aimesh->HasNormals()) {
          stride += 3;
          attributes |= SGL_NORMAL;
        }
        if (aimesh->HasTextureCoords(0)) {
          stride += 2;
          attributes |= SGL_TEXCOORD0;
        }
        uint vertexCount = aimesh->mNumVertices;
        float *vertices = new float[vertexCount * stride];
        uint indexCount = aimesh->mNumFaces * 3;
        uint *indices = new uint[indexCount];
        // get pointers to the position, normal and texture coordinates arrays
        aiVector3D *position = aimesh->mVertices;
        aiVector3D *normal = aimesh->mNormals;
        aiVector3D *texCoord = aimesh->mTextureCoords[0];
        float *vertexData = &vertices[0];
        AxisAlignedBoundingBox aabb;
        for (size_t j = 0; j < aimesh->mNumVertices; ++j) {
          // vertex position
          if (aimesh->HasPositions()) {
            *vertexData++ = position->x;
            *vertexData++ = position->y;
            *vertexData++ = position->z;
            // update aabb
            aabb.merge(position->x, position->y, position->z);
            // next vertex
            position++;
          }
          // vertex normal
          if (aimesh->HasNormals()) {
            *vertexData++ = normal->x;
            *vertexData++ = normal->y;
            *vertexData++ = normal->z;
            // next vertex
            normal++;
          }
          // texture coordinates
          if (aimesh->HasTextureCoords(0)) {
            *vertexData++ = texCoord->x;
            *vertexData++ = texCoord->y;
            // next vertex
            texCoord++;
          }
        }
        // get pointer to the face data
        aiFace *face = aimesh->mFaces;
        uint *indexData = &indices[0];
        for (size_t j = 0; j < aimesh->mNumFaces; ++j) {
          *indexData++ = face->mIndices[0];
          *indexData++ = face->mIndices[1];
          *indexData++ = face->mIndices[2];
          // next face
          face++;
        }
        // set mesh data
        SubMesh *subMesh = mesh->createSubMesh();
        subMesh->aabb().merge(aabb);
        subMesh->setMaterialName(materialNames[aimesh->mMaterialIndex]);
        subMesh->setVertexData(attributes, vertices, vertexCount, stride * 4);
        subMesh->setIndexData(indices, indexCount);
        // update mesh aabb
        mesh->aabb().merge(aabb);
        // free resources
        delete[] vertices;
        delete[] indices;
      }
    }
    // return mesh
    return mesh;
  }
}
