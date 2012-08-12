#include "Root.h"

#include "Camera.h"
#include "DirectionalLight.h"
#include "FrameBuffer.h"
#include "Instance.h"
#include "Light.h"
#include "Material.h"
#include "Mesh.h"
#include "PointLight.h"
#include "Program.h"
#include "Root.h"
#include "SceneNode.h"
#include "SpotLight.h"
#include "Texture.h"
#include "Viewport.h"
#include "Window.h"

#include <GL/glew.h>

#include <FreeImage.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <map>

namespace SimpleGL {
  static Root *_instance { nullptr };

  class RootPrivate {
  public:
    RootPrivate() {
      // create assimp importer
      importer = new Assimp::Importer();
    }

    ~RootPrivate() {
      delete importer;
      // delete windows
      for (uint i = 0; i < windows.size(); ++i)
        delete windows[i];
      // delete sceneNodes
      for (uint i = 0; i < sceneNodes.size(); ++i)
        delete sceneNodes[i];
      // delete lights
      for (uint i = 0; i < lights.size(); ++i)
        delete lights[i];
      // delete cameras
      for (uint i = 0; i < cameras.size(); ++i)
        delete cameras[i];
      // delete textures
      for (uint i = 0; i < textures.size(); ++i)
        delete textures[i];
      // delete programs
      for (uint i = 0; i < programs.size(); ++i)
        delete programs[i];
      // delete materials
      for (uint i = 0; i < materials.size(); ++i)
        delete materials[i];
      // delete meshes
      for (uint i = 0; i < meshes.size(); ++i)
        delete meshes[i];
      // delete instances
      for (uint i = 0; i < instances.size(); ++i)
        delete instances[i];
    }

    void calculateWorldTransforms(SceneNode *node) {
      // update node
      node->calculateWorldTransform();
      // visit child nodes
      for (uint i = 0; i < node->attachedNodes().size(); ++i)
        calculateWorldTransforms(node->attachedNodes().at(i));
    }

    void render(Camera *camera, SceneNode *node, const Matrix4f &viewProjMatrix) {
      // visit child nodes
      for (uint i = 0; i < node->attachedNodes().size(); ++i)
        render(camera, node->attachedNodes().at(i), viewProjMatrix);
      // render meshes
      for (uint i = 0; i < node->attachedObjects().size(); ++i) {
        Instance *instance = dynamic_cast<Instance *>(node->attachedObjects().at(i));
        if (!instance)
          continue;
        Mesh *mesh = Root::instance()->retrieveMesh(instance->mesh());
        if (!mesh)
          continue;
        Material *material = Root::instance()->retrieveMaterial(instance->material());
        if (!material)
          material = Root::instance()->retrieveMaterial(mesh->material());
        if (!material)
          continue;
        Program *program = Root::instance()->retrieveProgram(material->program());
        if (!program)
          continue;
        // bind the material
        material->bind();
        // set uniforms
        program->setUniform("ModelMatrix", node->worldTransform());
        program->setUniform("ModelViewProjMatrix", viewProjMatrix * node->worldTransform());
        // render the mesh
        mesh->render(camera);
        // unbind material
        material->unbind();
      }
    }

    Assimp::Importer *importer { nullptr };
    std::vector<Window *> windows;
    std::vector<SceneNode *> sceneNodes;
    std::vector<Light *> lights;
    std::vector<Camera *> cameras;
    std::vector<Texture *> textures;
    std::vector<Program *> programs;
    std::vector<Material *> materials;
    std::vector<Mesh *> meshes;
    std::vector<Instance *> instances;

    Vector2i mousePosition { 0, 0 };
    long time { 0 };

    long fpsTime { 0 };
    long fpsCount { 0 };
    float fps { 0.0f };
  };

  Root::Root() : d(new RootPrivate()) {
    // create root scene node
    d->sceneNodes.push_back(new SceneNode());
  }

  Root::~Root() {
    delete d;
  }

  void Root::initialize() {
    instance();
  }

  void Root::destroy() {
    delete _instance;
    _instance = 0;
  }

  Root *Root::instance() {
    if (_instance == 0)
      _instance = new Root();
    return _instance;
  }

  Window *Root::createWindow(int width, int height) {
    Window *window = new Window(width, height);
    // add to list
    d->windows.push_back(window);
    // return window
    return window;
  }

  const std::vector<Window *> &Root::windows() const {
    return d->windows;
  }

  SceneNode *Root::createSceneNode() {
    SceneNode *sceneNode = new SceneNode();
    // add to list
    d->sceneNodes.push_back(sceneNode);
    // return scene node
    return sceneNode;
  }

  SceneNode *Root::rootSceneNode() const {
    return d->sceneNodes.at(0);
  }

  const std::vector<SceneNode *> &Root::sceneNodes() const {
    return d->sceneNodes;
  }

  Light *Root::createLight(LightType type) {
    Light *light;
    switch (type) {
    case LT_DIRECTIONAL:
      light = new DirectionalLight();
      light->setMaterial("DirectionalLight");
      break;
    case LT_POINT:
      light = new PointLight();
      light->setMaterial("PointLight");
      break;
    case LT_SPOT:
      light = new SpotLight();
      light->setMaterial("SpotLight");
      break;
    }
    // add to list
    d->lights.push_back(light);
    // return light
    return light;
  }

  const std::vector<Light *> &Root::lights() const {
    return d->lights;
  }

  Camera *Root::createCamera() {
    Camera *camera = new Camera();
    // add to list
    d->cameras.push_back(camera);
    // return camera
    return camera;
  }

  const std::vector<Camera *> &Root::cameras() const {
    return d->cameras;
  }

  Texture *Root::createTexture(const String &name, const String &path) {
    std::cout << "Root::createTexture(\"" << name << "\", \"" << path << "\");" << std::endl;

    Texture *texture = new Texture(name, path);
    // add to list
    d->textures.push_back(texture);
    // return texture
    return texture;
  }

  Texture *Root::retrieveTexture(const String &name) {
    if (name == "")
      return 0;
    for (uint i = 0; i < d->textures.size(); ++i)
      if (d->textures.at(i)->name() == name)
        return d->textures.at(i);
    return 0;
  }

  const std::vector<Texture *> &Root::textures() const {
    return d->textures;
  }

  Program *Root::createProgram(const String &name) {
    std::cout << "Root::createProgram(\"" << name << "\");" << std::endl;

    Program *program = new Program(name);
    // add to list
    d->programs.push_back(program);
    // return program
    return program;
  }

  Program *Root::retrieveProgram(const String &name) {
    if (name == "")
      return 0;
    for (uint i = 0; i < d->programs.size(); ++i)
      if (d->programs.at(i)->name() == name)
        return d->programs.at(i);
    return 0;
  }

  const std::vector<Program *> &Root::programs() const {
    return d->programs;
  }

  Material *Root::createMaterial(const String &name) {
    std::cout << "Root::createMaterial(\"" << name << "\");" << std::endl;

    Material *material = new Material(name);
    // add to list
    d->materials.push_back(material);
    // return material
    return material;
  }

  Material *Root::retrieveMaterial(const String &name) {
    if (name == "")
      return 0;
    for (uint i = 0; i < d->materials.size(); ++i)
      if (d->materials.at(i)->name() == name)
        return d->materials.at(i);
    return 0;
  }

  const std::vector<Material *> &Root::materials() const {
    return d->materials;
  }

  Mesh *Root::createMesh(const String &name) {
    std::cout << "Root::createMesh(\"" << name << "\");" << std::endl;

    Mesh *mesh = new Mesh(name);
    // add to list
    d->meshes.push_back(mesh);
    // return mesh
    return mesh;
  }

  Mesh *Root::retrieveMesh(const String &name) {
    if (name == "")
      return 0;
    for (uint i = 0; i < d->meshes.size(); ++i)
      if (d->meshes.at(i)->name() == name)
        return d->meshes.at(i);
    return 0;
  }

  Mesh *Root::createQuad(const String &name, float width, float height) {
    Mesh *mesh = Root::instance()->createMesh(name);
    // calculate vertex and index data
    float vertices[] = {
      -1.0f * width, -1.0f * height, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
      +1.0f * width, -1.0f * height, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
      +1.0f * width, +1.0f * height, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
      -1.0f * width, +1.0f * height, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f
    };
    uint indices[] = { 0, 1, 2, 0, 2, 3 };
    // set vertex and index data
    mesh->setVertexData(AT_POSITION | AT_NORMAL | AT_TEXCOORD0, vertices, 4, 32);
    mesh->setIndexData(indices, 6);
    // return mesh
    return mesh;
  }

  Mesh *Root::createPlane(const String &name, float width, float height, float uTile, float vTile) {
    Mesh *mesh = Root::instance()->createMesh(name);
    // calculate vertex and index data
    float vertices[] = {
      -1.0f * width, 0.0f, +1.0f * height, 0.0f, 1.0f, 0.0f, -1.0f * uTile, -1.0f * vTile,
      +1.0f * width, 0.0f, +1.0f * height, 0.0f, 1.0f, 0.0f, +1.0f * uTile, -1.0f * vTile,
      +1.0f * width, 0.0f, -1.0f * height, 0.0f, 1.0f, 0.0f, +1.0f * uTile, +1.0f * vTile,
      -1.0f * width, 0.0f, -1.0f * height, 0.0f, 1.0f, 0.0f, -1.0f * uTile, +1.0f * vTile
    };
    uint indices[] = { 0, 1, 2, 0, 2, 3 };
    // set vertex and index data
    mesh->setVertexData(AT_POSITION | AT_NORMAL | AT_TEXCOORD0, vertices, 4, 32);
    mesh->setIndexData(indices, 6);
    // return mesh
    return mesh;
  }

  Mesh *Root::createCube(const String &name, float width, float height, float depth) {
    Mesh *mesh = Root::instance()->createMesh(name);
    // calculate vertex and index data
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
    mesh->setVertexData(AT_POSITION | AT_NORMAL | AT_TEXCOORD0, vertices, 24, 32);
    mesh->setIndexData(indices, 36);
    // return mesh
    return mesh;
  }

  Mesh *Root::createCone(const String &name, float radius, float height, uint slices, uint stacks) {
    Mesh *mesh = Root::instance()->createMesh(name);
    // calculate vertex and index data
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
        Vector3f vNormal = glm::normalize(Vector3f(x0, y0, height + z0));
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
    mesh->setVertexData(AT_POSITION | AT_NORMAL | AT_TEXCOORD0, vertices, vertexCount, 32);
    mesh->setIndexData(indices, indexCount);
    // clean up
    delete[] vertices;
    delete[] indices;
    // return mesh
    return mesh;
  }

  Mesh *Root::createSphere(const String &name, float radius, uint slices, uint stacks) {
    Mesh *mesh = Root::instance()->createMesh(name);
    // calculate vertex and index data
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
        Vector3f vNormal = glm::normalize(Vector3f(x0, y0, z0));
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
    mesh->setVertexData(AT_POSITION | AT_NORMAL | AT_TEXCOORD0, vertices, vertexCount, 32);
    mesh->setIndexData(indices, indexCount);
    // clean up
    delete[] vertices;
    delete[] indices;
    // return mesh
    return mesh;
  }

  Mesh *Root::loadMesh(const String &name, const String &path) {
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
                           aiProcess_OptimizeGraph);
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
      Material *material = Root::instance()->createMaterial(ainame.data);
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
    // import meshes
    std::map<uint, Mesh *> meshes;
    for (uint i = 0; i < scene->mNumMeshes; ++i) {
      const struct aiMesh *aimesh = scene->mMeshes[i];
      std::cout << "Mesh: " << aimesh->mName.data << std::endl;
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
      // set mesh data
      Mesh *mesh = Root::instance()->createMesh(name);
      mesh->setMaterial(materials[aimesh->mMaterialIndex]);
      mesh->setVertexData(attributes, vertices, vertexCount, stride * 4);
      mesh->setIndexData(indices, indexCount);
      // free resources
      delete[] vertices;
      delete[] indices;
      // add to list
      meshes[i] = mesh;
    }
    // return first mesh
    return meshes.at(0);
  }

  const std::vector<Mesh *> &Root::meshes() const {
    return d->meshes;
  }

  Instance *Root::createInstance(const String &mesh, const String &material) {
    Instance *instance = new Instance(mesh, material);
    // add to list
    d->instances.push_back(instance);
    // return instance
    return instance;
  }

  const std::vector<Instance *> &Root::instances() const {
    return d->instances;
  }

  void Root::renderOneFrame(long time) {
    // calculate time since last frame
    long millis = (d->time == 0) ? 0 : (time - d->time);
    d->time = time;
    // update fps
    d->fpsCount++;
    d->fpsTime += millis;
    // set general state
    glEnable(GL_TEXTURE_2D);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    // update scene transformations
    d->calculateWorldTransforms(d->sceneNodes.at(0));
    // start rendering
    for (Window *window: d->windows) {
      // clear color and depth buffers
      glDepthMask(GL_TRUE);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      // bind gbuffer for writing
      window->frameBuffer()->bind();
      // clear color and depth buffers
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      // unbind framebuffer
      window->frameBuffer()->unbind();
      // render through each viewport
      for (Viewport *viewport: window->viewports()) {
        // calculate viewport dimension in pixels;
        float left = viewport->left() * window->size().x;
        float top = viewport->top() * window->size().y;
        float width = viewport->width() * window->size().x;
        float height = viewport->height() * window->size().y;
        // set up viewport
        glViewport(left, top, width, height);
        // get viewport camera
        Camera *camera = viewport->camera();
        if (!camera)
          continue;
        // update camera's aspect ratio
        camera->setAspectRatio(width / height);
        // calculate view proj matrix
        Matrix4f viewProjMatrix = camera->projectionMatrix() * camera->viewMatrix();
        /////////////////////////////////////////////////////////////////////////////
        ////  GEOMETRY PASS
        /////////////////////////////////////////////////////////////////////////////
        window->frameBuffer()->bind();
        // enable face culling
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        // enable depth test
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
        // disable blending
        glDisable(GL_BLEND);
        // render scene
        d->render(camera, d->sceneNodes.at(0), viewProjMatrix);
        // unbind gbuffer
        window->frameBuffer()->unbind();
#if 0
        // blit the frame buffer
        window->frameBuffer()->blit();
#else
        /////////////////////////////////////////////////////////////////////////////
        ////  LIGHTING PASS
        /////////////////////////////////////////////////////////////////////////////
        // enable face culling
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        // disable depth test
        glDisable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);
        // enable blending
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_ONE, GL_ONE);
        // bind textures
        window->frameBuffer()->bindTextures();
        // render lights
        for (int type = LT_DIRECTIONAL; type <= LT_SPOT; ++type) {
          Program *program = 0;
          if (type == LT_DIRECTIONAL)
            program = Root::instance()->retrieveProgram("DirectionalLight");
          else if (type == LT_POINT)
            program = Root::instance()->retrieveProgram("PointLight");
          else if (type == LT_SPOT)
            program = Root::instance()->retrieveProgram("SpotLight");
          if (!program)
            continue;
          program->bind();
          program->setUniform("texture0", 0);
          program->setUniform("texture1", 1);
          program->setUniform("texture2", 2);
          program->setUniform("viewportSize", Vector2f(width, height));
          program->setUniform("cameraPos", camera->parentSceneNode()->worldPosition());
          // render the light
          for (uint k = 0; k < d->lights.size(); ++k)
            if (d->lights.at(k)->type() == type)
              d->lights.at(k)->render(camera);
          // deselect material
          program->unbind();
        }
        // unbind textures
        window->frameBuffer()->unbindTextures();
#endif
      }
    }
  }

  const float Root::fps() const {
    if (d->fpsTime >= 1000) {
      d->fps = d->fpsCount / (d->fpsTime * 0.001f);
      d->fpsCount = 0;
      d->fpsTime = 0;
      std::cout << "FPS: " << d->fps << std::endl;
    }
    // return fps
    return d->fps;
  }
}
