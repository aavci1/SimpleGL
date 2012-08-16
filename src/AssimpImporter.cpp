#include "AssimpImporter.h"

#include "Animation.h"
#include "AnimationTrack.h"
#include "Bone.h"
#include "Material.h"
#include "Mesh.h"
#include "Root.h"
#include "SubMesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <map>
#include <sstream>

namespace SimpleGL {
  class Vertex {
  public:
    Vector3f position { 0.0f, 0.0f, 0.0f };
    Vector3f normal { 0.0f, 0.0f, 0.0f };
    Vector3f tangent { 0.0f, 0.0f, 0.0f };
    Vector3f bitangent { 0.0f, 0.0f, 0.0f };
    Vector3f color { 0.0f, 0.0f, 0.0f };
    Vector2f texCoord0 { 0.0f, 0.0f };
    Vector2f texCoord1 { 0.0f, 0.0f };
    Vector2f texCoord2 { 0.0f, 0.0f };
    Vector2f texCoord3 { 0.0f, 0.0f };
    Vector4i boneIds { 0, 0, 0, 0 };
    Vector4f boneWeights { 0.0f, 0.0f, 0.0f, 0.0f };
  };

  class AssimpImporterPrivate {
  public:
    AssimpImporterPrivate() {

    }

    ~AssimpImporterPrivate() {
      delete importer;
    }

    Assimp::Importer *importer { new Assimp::Importer() };
    const aiScene *scene;
    Mesh *mesh;
    string path;
    string directory;
    map<int, SubMesh *> meshes;
    map<int, Material *> materials;

    const string tostring2(const int number) {
      stringstream ss;
      ss << number;
      return ss.str();
    }

    Vector3f toVector(aiVector3D aivector) {
      return Vector3f(aivector.x, aivector.y, aivector.z);
    }

    Quaternion toQuaternion(aiQuaternion aiquaternion) {
      return Quaternion(aiquaternion.w, aiquaternion.x, aiquaternion.y, aiquaternion.z);
    }

    Matrix4f toMatrix(aiMatrix4x4 aimatrix) {
      Matrix4f matrix;
      matrix[0][0] = aimatrix.a1;
      matrix[0][1] = aimatrix.b1;
      matrix[0][2] = aimatrix.c1;
      matrix[0][3] = aimatrix.d1;

      matrix[1][0] = aimatrix.a2;
      matrix[1][1] = aimatrix.b2;
      matrix[1][2] = aimatrix.c2;
      matrix[1][3] = aimatrix.d2;

      matrix[2][0] = aimatrix.a3;
      matrix[2][1] = aimatrix.b3;
      matrix[2][2] = aimatrix.c3;
      matrix[2][3] = aimatrix.d3;

      matrix[3][0] = aimatrix.a4;
      matrix[3][1] = aimatrix.b4;
      matrix[3][2] = aimatrix.c4;
      matrix[3][3] = aimatrix.d4;

      return matrix;
    }

    void importMaterial(uint index, string program) {
      if (index >= scene->mNumMaterials)
        return;
      aiMaterial *aimaterial = scene->mMaterials[index];
      // get material name
      aiString ainame;
      aimaterial->Get(AI_MATKEY_NAME, ainame);
      // TODO: get other material properties (specular, shininess etc.)
      //  create material
      Material *material = Root::instance()->createMaterial(directory + "$mat" + tostring2(index));
      material->setProgram(program);
      // put material into the list
      materials[index] = material;
      // extract diffuse maps
      for (uint j = 0; j < aimaterial->GetTextureCount(aiTextureType_DIFFUSE); ++j) {
        aiString aitexturepath;
        // get texture path
        aimaterial->GetTexture(aiTextureType_DIFFUSE, j, &aitexturepath);
        // generate texture path
        string texturePath = aitexturepath.data;
        texturePath = directory + "/" + texturePath.substr(texturePath.find_last_of("/") + 1);
        // create texture
        Root::instance()->createTexture(texturePath, texturePath);
        // add texture to the material
        material->addTexture(texturePath);
      }
    }

    void importMesh(Bone *parent, uint index) {
      if (index >= scene->mNumMeshes)
        return;
      aiMesh *aimesh = scene->mMeshes[index];
      SubMesh *subMesh = mesh->createSubMesh(parent);
      meshes[index] = subMesh;
      // create vertex buffer
      uint vertexCount = aimesh->mNumVertices;
      Vertex *vertices = new Vertex[vertexCount];
      // read vertices
      for (uint i = 0; i < vertexCount; ++i) {
        if (aimesh->HasPositions()) {
          vertices[i].position.x = aimesh->mVertices[i].x;
          vertices[i].position.y = aimesh->mVertices[i].y;
          vertices[i].position.z = aimesh->mVertices[i].z;
        }
        if (aimesh->HasNormals()) {
          vertices[i].normal.x = aimesh->mNormals[i].x;
          vertices[i].normal.y = aimesh->mNormals[i].y;
          vertices[i].normal.z = aimesh->mNormals[i].z;
        }
        if (aimesh->HasTangentsAndBitangents()) {
          vertices[i].tangent.x = aimesh->mTangents[i].x;
          vertices[i].tangent.y = aimesh->mTangents[i].y;
          vertices[i].tangent.z = aimesh->mTangents[i].z;
          vertices[i].bitangent.x = aimesh->mBitangents[i].x;
          vertices[i].bitangent.y = aimesh->mBitangents[i].y;
          vertices[i].bitangent.z = aimesh->mBitangents[i].z;
        }
        if (aimesh->HasTextureCoords(0)) {
          vertices[i].texCoord0.x = aimesh->mTextureCoords[0][i].x;
          vertices[i].texCoord0.y = aimesh->mTextureCoords[0][i].y;
        }
        if (aimesh->HasTextureCoords(1)) {
          vertices[i].texCoord1.x = aimesh->mTextureCoords[1][i].x;
          vertices[i].texCoord1.y = aimesh->mTextureCoords[1][i].y;
        }
        if (aimesh->HasTextureCoords(2)) {
          vertices[i].texCoord2.x = aimesh->mTextureCoords[2][i].x;
          vertices[i].texCoord2.y = aimesh->mTextureCoords[2][i].y;
        }
        if (aimesh->HasTextureCoords(3)) {
          vertices[i].texCoord3.x = aimesh->mTextureCoords[3][i].x;
          vertices[i].texCoord3.y = aimesh->mTextureCoords[3][i].y;
        }
      }
      // read bone weights
      for (uint i = 0; i < aimesh->mNumBones; ++i) {
        aiBone *aibone = aimesh->mBones[i];
        uint boneId = 0;
        for (uint j = 0; j < mesh->numBones(); ++j)
          if (mesh->boneAt(j) && (mesh->boneAt(j)->name().compare(aibone->mName.data) == 0)) {
            boneId = j;
            break;
          }
        if (boneId == 0)
          continue;
        // set bones offset matrix
        mesh->boneAt(boneId)->setOffsetMatrix(toMatrix(aibone->mOffsetMatrix));

        for (uint j = 0; j < aibone->mNumWeights; ++j) {
          aiVertexWeight vertexWeight = aibone->mWeights[j];
          for (uint k = 0; k < 4; ++k) {
            if (vertices[vertexWeight.mVertexId].boneIds[k] == 0) {
              vertices[vertexWeight.mVertexId].boneIds[k] = boneId;
              vertices[vertexWeight.mVertexId].boneWeights[k] = vertexWeight.mWeight;
              break;
            }
          }
        }
      }
      // create index buffer
      uint indexCount = aimesh->mNumFaces * 3;
      uint *indices = new uint[indexCount];
      // read indices
      for (uint i = 0; i < aimesh->mNumFaces; ++i) {
        indices[i * 3 + 0] = aimesh->mFaces[i].mIndices[0];
        indices[i * 3 + 1] = aimesh->mFaces[i].mIndices[1];
        indices[i * 3 + 2] = aimesh->mFaces[i].mIndices[2];
      }
      // set mesh data
      subMesh->setVertexData(vertices, vertexCount, AT_POSITION | AT_NORMAL | AT_TANGENT_AND_BITANGENT | AT_COLOR | AT_TEXCOORD0 | AT_TEXCOORD1 | AT_TEXCOORD2 | AT_TEXCOORD3 | AT_BONES);
      subMesh->setIndexData(indices, indexCount);
      // set material
      if (materials[aimesh->mMaterialIndex] == nullptr)
        importMaterial(aimesh->mMaterialIndex, aimesh->HasBones() ? "Skinned" : "Textured");
      if (materials[aimesh->mMaterialIndex] != nullptr)
        subMesh->setMaterial(materials[aimesh->mMaterialIndex]->name());
    }

    void importNode(aiNode *ainode, Bone *parent) {
      Bone *node = mesh->createBone(parent);
      // import node info
      if (parent)
        parent->children().push_back(node);
      node->setName(string(ainode->mName.data));
      node->setTransform(toMatrix(ainode->mTransformation));
      // import meshes
      for (uint32_t i = 0; i < ainode->mNumMeshes; ++i) {
        int meshIndex = ainode->mMeshes[i];
        if (meshes[meshIndex] == nullptr)
          importMesh(node, meshIndex);
        if (meshes[meshIndex] != nullptr)
          node->subMeshes().push_back(meshes[meshIndex]);
      }
      // import children
      for (uint32_t i = 0; i < ainode->mNumChildren; ++i)
        importNode(ainode->mChildren[i], node);
    }

    AnimationTrack *importChannel(aiNodeAnim *_channel, float ticksPerSecond) {
      AnimationTrack *track = new AnimationTrack(_channel->mNodeName.data);
      for (uint i = 0; i < _channel->mNumPositionKeys; ++i)
        track->addPositionKey(_channel->mPositionKeys[i].mTime / ticksPerSecond * 1000, toVector(_channel->mPositionKeys[i].mValue));
      for (uint i = 0; i < _channel->mNumRotationKeys; ++i)
        track->addOrientationKey(_channel->mRotationKeys[i].mTime / ticksPerSecond * 1000, toQuaternion(_channel->mRotationKeys[i].mValue));
      for (uint i = 0; i < _channel->mNumScalingKeys; ++i)
        track->addScaleKey(_channel->mScalingKeys[i].mTime / ticksPerSecond * 1000, toVector(_channel->mScalingKeys[i].mValue));
      return track;
    }

    void importAnimation(uint index) {
      if (index > scene->mNumAnimations)
        return;
      aiAnimation *_animation = scene->mAnimations[index];
      Animation *animation = mesh->createAnimation(_animation->mName.data);
      double ticksPerSecond = _animation->mTicksPerSecond;
      if (ticksPerSecond == 0)
        ticksPerSecond = 10;
      animation->setDuration((_animation->mDuration / ticksPerSecond) * 1000);
      for(uint i = 0; i < _animation->mNumChannels; ++i)
        animation->tracks().push_back(importChannel(_animation->mChannels[i], ticksPerSecond));
    }
  };

  AssimpImporter::AssimpImporter() : d(new AssimpImporterPrivate()) {
  }

  AssimpImporter::~AssimpImporter() {
    delete d;
  }

  Mesh *AssimpImporter::import(const string &name, const string &path) {
    // import scene
    d->scene = d->importer->ReadFile(path.c_str(),
                                                 aiProcess_CalcTangentSpace |
                                                 aiProcess_JoinIdenticalVertices |
                                                 aiProcess_Triangulate |
                                                 aiProcess_GenSmoothNormals |
                                                 aiProcess_SplitLargeMeshes |
                                                 aiProcess_LimitBoneWeights |
                                                 aiProcess_ImproveCacheLocality |
                                                 aiProcess_RemoveRedundantMaterials |
                                                 aiProcess_FixInfacingNormals |
                                                 aiProcess_SortByPType |
                                                 aiProcess_FindDegenerates |
                                                 aiProcess_FindInvalidData |
                                                 aiProcess_GenUVCoords |
                                                 aiProcess_TransformUVCoords |
                                                 aiProcess_FindInstances |
                                                 aiProcess_OptimizeMeshes |
                                                 aiProcess_OptimizeGraph);
    // check status
    if (!d->scene)
      return nullptr;
    // extract base directory
    d->mesh = Root::instance()->createMesh(name);
    d->path = path;
    d->directory = path.substr(0, path.find_last_of("/"));
    // import nodes
    d->importNode(d->scene->mRootNode, d->mesh->boneAt(0));
    // TODO: import textures
    // import animations
    for (uint32_t i = 0; i < d->scene->mNumAnimations; ++i)
      d->importAnimation(i);
    // return mesh
    return d->mesh;
  }
}
