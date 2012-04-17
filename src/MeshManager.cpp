#include "MeshManager.h"

#include "Attribute.h"
#include "Material.h"
#include "MaterialManager.h"
#include "Mesh.h"
#include "SubMesh.h"

#include <assimp/assimp.hpp>
#include <assimp/aiScene.h>
#include <assimp/aiPostProcess.h>

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

  Mesh *MeshManager::loadMesh(const std::string &path) {
    Mesh *mesh = new Mesh();
    // try loading the mesh using assimp
    const aiScene *scene = d->importer->ReadFile(path.c_str(), aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_PreTransformVertices | aiProcess_FixInfacingNormals);
    // return mesh if scene cannot be loaded
    if (!scene) {
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
        // check diffuse texture
        if (material->GetTextureCount(aiTextureType_DIFFUSE) <= 0)
          continue;
        // get texture path
        aiString texturePath;
        if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) != AI_SUCCESS)
          continue;
        // make up texture path
        std::string texturePathString = dir + "/" + texturePath.data;
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
        for (size_t j = 0; j < aimesh->mNumVertices; ++j) {
          // vertex position
          if (aimesh->HasPositions()) {
            *vertexData++ = position->x;
            *vertexData++ = position->y;
            *vertexData++ = position->z;
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
        subMesh->setMaterialName(materialNames[aimesh->mMaterialIndex]);
        subMesh->setVertexData(attributes, vertices, vertexCount, stride * 4);
        subMesh->setIndexData(indices, indexCount);
        // free resources
        delete[] vertices;
        delete[] indices;
      }
    }
    // return mesh
    return mesh;
  }
}
