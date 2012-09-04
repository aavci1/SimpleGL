#include "AssimpImporter.h"

#include "Animation.h"
#include "AnimationTrack.h"
#include "Bone.h"
#include "Material.h"
#include "Mesh.h"
#include "Model.h"
#include "Root.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <map>
#include <sstream>

using namespace SimpleGL;

namespace AssimpImporter {
  class Vertex {
  public:
    Vector3f position { 0.0f, 0.0f, 0.0f };
    Vector3f normal { 0.0f, 0.0f, 0.0f };
    Vector3f tangent { 0.0f, 0.0f, 0.0f };
    Vector3f bitangent { 0.0f, 0.0f, 0.0f };
    Vector2f texCoord0 { 0.0f, 0.0f };
    Vector2f texCoord1 { 0.0f, 0.0f };
    Vector2f texCoord2 { 0.0f, 0.0f };
    Vector2f texCoord3 { 0.0f, 0.0f };
    Vector4i boneIds { 0, 0, 0, 0 };
    Vector4f boneWeights { 0.0f, 0.0f, 0.0f, 0.0f };
  };

  const string toString(const int number) {
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

  MaterialPtr importMaterial(aiMaterial *aimaterial, string baseDir, string name) {
    // TODO: get other material properties (specular, shininess etc.)
    //  create material
    MaterialPtr material = Root::instance()->createMaterial(name);
    for (aiTextureType i: { aiTextureType_DIFFUSE, aiTextureType_SPECULAR, aiTextureType_NORMALS }) {
      // extract diffuse maps
      for (uint j = 0; j < aimaterial->GetTextureCount(i); ++j) {
        aiString aitexturepath;
        // get texture path
        aimaterial->GetTexture(i, j, &aitexturepath);
        // generate texture path
        string texturePath = aitexturepath.data;
        texturePath = baseDir + "/" + texturePath.substr(texturePath.find_last_of("/") + 1);
        // add texture to the material
        material->addTexture(texturePath);
      }
    }
    // return material
    return material;
  }

  void importModel(const aiScene *scene, ModelPtr  model, map<int, MaterialPtr> materials, uint index) {
    if (index >= scene->mNumMeshes)
      return;
    aiMesh *aimesh = scene->mMeshes[index];
    MeshPtr mesh = model->createMesh();
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
      for (uint j = 0; j < model->bones().size(); ++j)
        if (model->bones().at(j) && (model->bones().at(j)->name().compare(aibone->mName.data) == 0)) {
          boneId = j;
          break;
        }
      if (boneId == 0)
        continue;
      // set bones offset matrix
      model->bones().at(boneId)->setOffsetMatrix(toMatrix(aibone->mOffsetMatrix));

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
    mesh->setVertexData((float *)vertices, vertexCount, AT_POSITION | AT_NORMAL | AT_TANGENT_AND_BITANGENT | AT_TEXCOORD0 | AT_TEXCOORD1 | AT_TEXCOORD2 | AT_TEXCOORD3 | AT_BONES);
    mesh->setIndexData(indices, indexCount);
    // set material
    if (materials[aimesh->mMaterialIndex] != nullptr) {
      materials[aimesh->mMaterialIndex]->setProgram(aimesh->HasBones() ? "Skinned" : "Textured");
      mesh->setMaterial(materials[aimesh->mMaterialIndex]->name());
    }
  }

  void importNode(aiNode *_node, ModelPtr  model, BonePtr parent) {
    BonePtr bone = model->createBone(string(_node->mName.data));
    // import node info
    if (parent)
      parent->attach(bone);
    bone->setTransform(toMatrix(_node->mTransformation));
    // import children
    for (uint i = 0; i < _node->mNumChildren; ++i)
      importNode(_node->mChildren[i], model, bone);
  }

  AnimationTrackPtr importChannel(AnimationPtr animation, aiNodeAnim *_channel, float ticksPerSecond) {
    AnimationTrackPtr track = animation->createTrack(_channel->mNodeName.data);
    for (uint i = 0; i < _channel->mNumPositionKeys; ++i)
      track->createPositionKey(_channel->mPositionKeys[i].mTime / ticksPerSecond * 1000, toVector(_channel->mPositionKeys[i].mValue));
    for (uint i = 0; i < _channel->mNumRotationKeys; ++i)
      track->createOrientationKey(_channel->mRotationKeys[i].mTime / ticksPerSecond * 1000, toQuaternion(_channel->mRotationKeys[i].mValue));
    for (uint i = 0; i < _channel->mNumScalingKeys; ++i)
      track->createScaleKey(_channel->mScalingKeys[i].mTime / ticksPerSecond * 1000, toVector(_channel->mScalingKeys[i].mValue));
    return track;
  }

  void importAnimation(const aiScene *scene, ModelPtr  model, uint index) {
    aiAnimation *_animation = scene->mAnimations[index];
    AnimationPtr animation = model->createAnimation(_animation->mName.data);
    double ticksPerSecond = _animation->mTicksPerSecond;
    if (ticksPerSecond == 0)
      ticksPerSecond = 10;
    animation->setDuration((_animation->mDuration / ticksPerSecond) * 1000);
    for (uint i = 0; i < _animation->mNumChannels; ++i)
      importChannel(animation, _animation->mChannels[i], ticksPerSecond);
  }

  ModelPtr  import(const string &name, const string &path) {
    Assimp::Importer importer;
    // import scene
    const aiScene *scene = importer.ReadFile(path.c_str(),
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
    if (!scene)
      return nullptr;

    ModelPtr model = Root::instance()->createModel(name);
    string baseDir = path.substr(0, path.find_last_of("/"));
    map<int, MaterialPtr> materials;
    // TODO: import textures
    // import materials
    for (uint i = 0; i < scene->mNumMaterials; ++i)
      materials[i] = importMaterial(scene->mMaterials[i], baseDir, path + "$mat" + toString(i));
    // import nodes
    importNode(scene->mRootNode, model, nullptr);
    // import meshes
    for (uint i = 0; i < scene->mNumMeshes; ++i)
      importModel(scene, model, materials, i);
    // import animations
    for (uint i = 0; i < scene->mNumAnimations; ++i)
      importAnimation(scene, model, i);
    // return model
    return model;
  }
}
