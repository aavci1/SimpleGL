#include "AssimpImporter.h"

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
  class AssimpImporterPrivate {
  public:
    AssimpImporterPrivate() {

    }

    ~AssimpImporterPrivate() {
      delete importer;
    }

    Assimp::Importer *importer { new Assimp::Importer() };
  };

  AssimpImporter::AssimpImporter() : d(new AssimpImporterPrivate()) {
  }

  AssimpImporter::~AssimpImporter() {
    delete d;
  }

  const std::string toString2(const int number) {
    std::stringstream ss;
    ss << number;
    return ss.str();
  }

  void dumpNodes(aiNode *node, String string) {
    std::cout << string << node->mName.data << " (" << node->mNumMeshes << " meshes)" << std::endl;
    for (uint i = 0; i < node->mNumChildren; ++i)
      dumpNodes(node->mChildren[i], string + "  ");
  }

  Mesh *AssimpImporter::loadMesh(const String &name, const String &path) {
    Mesh *mesh = Root::instance()->createMesh(name);
    // import scene
    const aiScene *scene = d->importer->ReadFile(path.c_str(),
                                                 aiProcess_CalcTangentSpace |
                                                 aiProcess_JoinIdenticalVertices |
                                                 aiProcess_Triangulate |
                                                 // aiProcess_RemoveComponent |
                                                 aiProcess_GenSmoothNormals |
                                                 aiProcess_SplitLargeMeshes |
                                                 aiProcess_PreTransformVertices |
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
                                                 aiProcess_OptimizeGraph |
                                                 aiProcess_Debone);
    // return mesh if scene cannot be loaded
    if (!scene) {
      std::cerr << "error: can not load model " << path << std::endl;
      return 0;
    }
    // extract base directory
    String directory = path.substr(0, path.find_last_of("/"));
    // import materials
    std::map<int, String> materials;
    for (uint i = 0; i < scene->mNumMaterials; ++i) {
      const aiMaterial *aimaterial = scene->mMaterials[i];
      // get material name
      aiString ainame;
      aimaterial->Get(AI_MATKEY_NAME, ainame);
      // TODO: get other material properties (specular, shininess etc.)
      //  create material
      Material *material = Root::instance()->createMaterial(path + "$mat" + toString2(i));
      // TODO: make program configurable
      material->setProgram("Textured");
      // add to list
      materials[i] = material->name();
      // extract diffuse maps
      for (uint j = 0; j < aimaterial->GetTextureCount(aiTextureType_DIFFUSE); ++j) {
        aiString aitexturepath;
        // get texture path
        aimaterial->GetTexture(aiTextureType_DIFFUSE, j, &aitexturepath);
        // generate texture path
        String texturePath = aitexturepath.data;
        texturePath = directory + "/" + texturePath.substr(texturePath.find_last_of("/") + 1);
        // create texture
        Root::instance()->createTexture(texturePath, texturePath);
        // add texture to the material
        material->addTexture(texturePath);
      }
    }
    dumpNodes(scene->mRootNode, "");
    // import meshes
    for (uint i = 0; i < scene->mNumMeshes; ++i) {
      const struct aiMesh *aimesh = scene->mMeshes[i];
      std::cout << "Mesh: " << aimesh->mName.data << std::endl;
      for (uint j = 0; j < aimesh->mNumBones; ++j) {

      }
      // skip meshes with non triangle primitives
      if (aimesh->mPrimitiveTypes != aiPrimitiveType_TRIANGLE)
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
      // get vertex count
      uint vertexCount = aimesh->mNumVertices;
      float *vertices = new float[vertexCount * stride];
      // get index count
      uint indexCount = aimesh->mNumFaces * 3;
      uint *indices = new uint[indexCount];
      // get pointers to the position, normal and texture coordinates arrays
      aiVector3D *position = aimesh->mVertices;
      aiVector3D *normal = aimesh->mNormals;
      aiVector3D *texCoord = aimesh->mTextureCoords[0];
      float *vertexData = &vertices[0];
      // extract vertex data
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
      // extract index data
      for (size_t j = 0; j < aimesh->mNumFaces; ++j) {
        *indexData++ = face->mIndices[0];
        *indexData++ = face->mIndices[1];
        *indexData++ = face->mIndices[2];
        // next face
        face++;
      }
      // create a submesh
      SubMesh *subMesh = mesh->createSubMesh();
      subMesh->setMaterial(materials[aimesh->mMaterialIndex]);
      subMesh->setVertexData(attributes, vertices, vertexCount, stride * 4);
      subMesh->setIndexData(indices, indexCount);
      // free resources
      delete[] vertices;
      delete[] indices;
    }
    // import animations
    for (uint i = 0; i < scene->mNumAnimations; ++i) {
      aiAnimation *animation = scene->mAnimations[i];
      std::cout << "Animation: " << animation->mName.data << std::endl;
      std::cout << "  Duration: " << animation->mDuration << std::endl;
      std::cout << "  Ticks per second: " << animation->mTicksPerSecond << std::endl;
      std::cout << "  Channel count: " << animation->mNumChannels << std::endl;
      for (int j = 0; j < animation->mNumChannels; ++j) {
        aiNodeAnim *channel = animation->mChannels[j];
        std::cout << "    Channel: " << channel->mNodeName.data << std::endl;
      }
      std::cout << "  Mesh channel count: " << animation->mNumMeshChannels << std::endl;
    }

    // return first mesh
    return mesh;
  }
}