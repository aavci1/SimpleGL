#include "MeshManager.h"

#include "AxisAlignedBoundingBox.h"
#include "Material.h"
#include "MaterialManager.h"
#include "Mesh.h"
#include "SubMesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <map>

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
        +1.0f * width, -1.0f * height, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        +1.0f * width, +1.0f * height, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -1.0f * width, +1.0f * height, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f
      };
      uint indices[] = { 0, 1, 2, 0, 2, 3 };
      // set vertex and index data
      subMesh->setVertexData(AT_POSITION | AT_NORMAL | AT_TEXCOORD0, vertices, 4, 32);
      subMesh->setIndexData(indices, 6);
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
      subMesh->setVertexData(AT_POSITION | AT_NORMAL | AT_TEXCOORD0, vertices, 4, 32);
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
      subMesh->setVertexData(AT_POSITION | AT_NORMAL | AT_TEXCOORD0, vertices, 24, 32);
      subMesh->setIndexData(indices, 36);
    }

    void createCone(SubMesh *subMesh, float radius, float height, uint slices, uint stacks) {
      // allocate the vertex buffer
      uint vertexCount = (stacks + 1) * (slices + 1) + 1 + slices + 1;
      float *vertices = new float[vertexCount * 8];
      uint indexCount = 6 * stacks * (slices + 1) + 3 * slices;
      uint *indices = new uint[indexCount];
      // vertex pointer
      float *pVertex = vertices;
      // index pointer
      uint *pIndices = indices;
      // index of the vertice for current face
      uint wVerticeIndex = 0 ;
      // allocate index buffer
      float fDeltaSegAngle = (2 * M_PI / slices);
      // Generate the group of rings for the sphere
      for (uint ring = 0; ring <= stacks; ring++) {
        float r0 = radius * ring / stacks;
        float z0 = -height * ring / stacks;
        // Generate the group of segments for the current ring
        for (uint seg = 0; seg <= slices; seg++) {
          float x0 = r0 * sinf(seg * fDeltaSegAngle);
          float y0 = r0 * cosf(seg * fDeltaSegAngle);
          // add vertex position
          *pVertex++ = x0;
          *pVertex++ = y0;
          *pVertex++ = z0;
          // add vertex normal
          glm::vec3 vNormal = glm::normalize(glm::vec3(x0, y0, height + z0));
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
            *pIndices++ = wVerticeIndex + slices;
            *pIndices++ = wVerticeIndex;
            *pIndices++ = wVerticeIndex + slices + 1;
            *pIndices++ = wVerticeIndex;
            *pIndices++ = wVerticeIndex + 1;
            // next vertice
            wVerticeIndex ++;
          }
        }
      }
      uint capCenterIndex = (stacks + 1) * (slices + 1);
      // add vertex position
      *pVertex++ = 0;
      *pVertex++ = 0;
      *pVertex++ = -height;
      // add vertex normal
      *pVertex++ = 0;
      *pVertex++ = 0;
      *pVertex++ = -1;
      // add texture coordinate
      *pVertex++ = 0;
      *pVertex++ = 0;
      for (uint seg = 0; seg <= slices; seg++) {
        float x0 = radius * sinf(seg * fDeltaSegAngle);
        float y0 = radius * cosf(seg * fDeltaSegAngle);
        // add vertex position
        *pVertex++ = x0;
        *pVertex++ = y0;
        *pVertex++ = -height;
        // add vertex normal
        *pVertex++ = 0;
        *pVertex++ = 0;
        *pVertex++ = -1;
        // add texture coordinate
        *pVertex++ = sinf(seg * fDeltaSegAngle);
        *pVertex++ = cosf(seg * fDeltaSegAngle);
        // add indices
        if (seg != slices) {
          *pIndices++ = capCenterIndex;
          *pIndices++ = capCenterIndex + seg + 1;
          *pIndices++ = capCenterIndex + seg + 2;
        }
      }
      // set vertex and index data
      subMesh->setVertexData(AT_POSITION | AT_NORMAL | AT_TEXCOORD0, vertices, vertexCount, 32);
      subMesh->setIndexData(indices, indexCount);
      // clean up
      delete[] vertices;
      delete[] indices;
    }

    void createSphere(SubMesh *subMesh, float radius, uint slices, uint stacks) {
      // allocate the vertex buffer
      uint vertexCount = (stacks + 1) * (slices + 1);
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
      for (uint ring = 0; ring <= stacks; ring++) {
        float r0 = radius * sinf(ring * fDeltaRingAngle);
        float y0 = radius * cosf(ring * fDeltaRingAngle);
        // Generate the group of segments for the current ring
        for (uint seg = 0; seg <= slices; seg++) {
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
      subMesh->setVertexData(AT_POSITION | AT_NORMAL | AT_TEXCOORD0, vertices, vertexCount, 32);
      subMesh->setIndexData(indices, indexCount);
      // clean up
      delete[] vertices;
      delete[] indices;
    }

    std::vector<std::string> getTexture(const aiMaterial *material, aiTextureType type) {
      std::vector<std::string> textures;
      // get textures
      for (uint i = 0; i < material->GetTextureCount(type); ++i) {
        // get texture path
        aiString aistr;
        if (material->GetTexture(type, i, &aistr) != AI_SUCCESS)
          continue;
        // convert to regular string
        std::string texturePath = std::string(aistr.data);
        // remove directory part
        int index = texturePath.find_last_of("/");
        if (index != -1)
          texturePath = texturePath.substr(index + 1);
        // add to list
        textures.push_back(texturePath);
      }
      return textures;
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

  Mesh *MeshManager::createCone(float radius, float height, uint slices, uint stacks) {
    Mesh *mesh = new Mesh();
    // create a submesh
    SubMesh *subMesh = mesh->createSubMesh();
    // fill the sub mesh
    d->createCone(subMesh, radius, height, slices, stacks);
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
    if (!scene) {
      std::cerr << "error: can not load model " << path << std::endl;
      return mesh;
    }
    // material names
    std::map<int, std::string> materialNames;
    // load materials
    if (scene->HasMaterials()) {
      // get directory of the mesh
      std::string dir = path.substr(0, path.find_last_of("/"));
      // Initialize the materials
      for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
        const aiMaterial* material = scene->mMaterials[i];
        aiString name;
        material->Get(AI_MATKEY_NAME, name);
        // create material
        Material *mat = MaterialManager::instance()->getMaterialByName(name.data);
        // get diffuse maps
        std::vector<std::string> diffuseMaps = d->getTexture(material, aiTextureType_DIFFUSE);
        for (uint j = 0;  j < diffuseMaps.size(); ++j)
          mat->addTexture(dir + "/" + diffuseMaps.at(j));
        // set program
        mat->setProgram("media/textured.vert", "media/textured.frag");
        // set material index
        materialNames[i] = mat->name();
      }
    }
    // load meshes
    if (scene->HasMeshes()) {
      for (uint i = 0; i < scene->mNumMeshes; ++i) {
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
          attributes |= AT_POSITION;
        }
        if (aimesh->HasNormals()) {
          stride += 3;
          attributes |= AT_NORMAL;
        }
        if (aimesh->HasTextureCoords(0)) {
          stride += 2;
          attributes |= AT_TEXCOORD0;
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
