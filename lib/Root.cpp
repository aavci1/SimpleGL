#include "Root.h"

#include "Animation.h"
#include "AnimationTrack.h"
#include "Bone.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "InputStream.h"
#include "Instance.h"
#include "Light.h"
#include "Material.h"
#include "Mesh.h"
#include "Model.h"
#include "OutputStream.h"
#include "PointLight.h"
#include "Program.h"
#include "Root.h"
#include "SceneNode.h"
#include "SpotLight.h"
#include "Viewport.h"
#include "Window.h"

#include <GL/glew.h>

#include <algorithm>
#include <fstream>
#include <map>
#include <queue>

namespace SimpleGL {
  static Root *_instance { nullptr };

  void initialize() {
    _instance = new Root();
  }

  void destroy() {
    delete _instance;
    // nullify instance
    _instance = nullptr;
  }

  class RenderOp {
  public:
    RenderOp(MeshPtr mesh, string material, Matrix4f transform, const uint numBones, const float *boneTransforms) {
      this->mesh = mesh;
      this->material = material;
      this->transform = transform;
      this->numBones = numBones;
      this->boneTransforms = boneTransforms;
    }

    MeshPtr mesh { nullptr };
    string material;
    Matrix4f transform;
    uint numBones { 0 };
    const float *boneTransforms { nullptr };
  };

  class RootPrivate {
  public:
    RootPrivate() {
    }

    ~RootPrivate() {
    }

    vector<WindowPtr> windows;
    SceneNodePtr rootSceneNode { new SceneNode() };
    vector<SceneNodePtr> sceneNodes;
    vector<InstancePtr> instances;
    vector<LightPtr> lights;
    map<string, CameraPtr> cameras;
    map<string, ModelPtr> models;
    map<string, MaterialPtr> materials;
    map<string, ProgramPtr> programs;

    vector<RenderOp> renderQueue;

    Vector2i mousePosition { 0, 0 };

    long fpsTime { 0 };
    long fpsCount { 0 };
    float fps { 0.0f };
  };

  Root::Root() : d(new RootPrivate()) {
    // initialize glew
    glewInit();
  }

  Root::~Root() {
    delete d;
  }

  Root *Root::instance() {
    return _instance;
  }

  WindowPtr Root::createWindow(int width, int height) {
    WindowPtr window(new Window(width, height));
    // add to list
    d->windows.push_back(window);
    // return window
    return window;
  }

  SceneNodePtr Root::createSceneNode() {
    SceneNodePtr sceneNode { new SceneNode() };
    // add to list
    d->sceneNodes.push_back(sceneNode);
    // return scene node
    return sceneNode;
  }

  SceneNodePtr Root::rootSceneNode() const {
    return d->rootSceneNode;
  }

  LightPtr Root::createLight(const string &type) {
    LightPtr light;
    if (type == "Light/Point")
      light = LightPtr(new PointLight());
    else if (type == "Light/Spot")
      light = LightPtr(new SpotLight());
    else if (type == "Light/Directional")
      light = LightPtr(new DirectionalLight());
    // add to list
    d->lights.push_back(light);
    // return light
    return light;
  }

  CameraPtr Root::createCamera(const string &name) {
    CameraPtr camera { new Camera(name) };
    // add to list
    if (!name.empty())
      d->cameras[name] = camera;
    // return camera
    return camera;
  }

  CameraPtr Root::retrieveCamera(const string &name) {
    if (name.empty() || d->cameras.count(name) == 0)
      return nullptr;
    return d->cameras[name];
  }

  void Root::removeCamera(const string &name) {
    if (d->cameras.count(name) == 0)
      return;
    d->cameras.erase(name);
  }

  ProgramPtr Root::createProgram(const string &name) {
    ProgramPtr program { new Program(name) };
    // add to list
    if (!name.empty())
      d->programs[name] = program;
    // return program
    return program;
  }

  ProgramPtr Root::retrieveProgram(const string &name) {
    if (name.empty() || d->programs.count(name) == 0)
      return nullptr;
    return d->programs[name];
  }

  void Root::removeProgram(const string &name) {
    if (d->programs.count(name) == 0)
      return;
    d->programs.erase(name);
  }

  MaterialPtr Root::createMaterial(const string &name) {
    MaterialPtr material { new Material(name) };
    // add to list
    if (!name.empty())
      d->materials[name] = material;
    // return material
    return material;
  }

  MaterialPtr Root::retrieveMaterial(const string &name) {
    if (name.empty() || d->materials.count(name) == 0)
      return nullptr;
    return d->materials[name];
  }

  void Root::removeMaterial(const string &name) {
    if (d->materials.count(name) == 0)
      return;
    d->materials.erase(name);
  }

  ModelPtr Root::createModel(const string &name) {
    ModelPtr model { new Model(name) };
    // add to list
    if (!name.empty())
      d->models[name] = model;
    // return model
    return model;
  }

  ModelPtr Root::retrieveModel(const string &name) {
    if (name.empty() || d->models.count(name) == 0)
      return nullptr;
    return d->models[name];
  }

  void Root::removeModel(const string &name) {
    if (d->models.count(name) == 0)
      return;
    d->models.erase(name);
  }

  ModelPtr Root::createQuad(const string &name, float width, float height) {
    ModelPtr model = Root::instance()->createModel(name);
    // calculate vertex and index data
    float vertices[] = {
      -1.0f * width, -1.0f * height, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
      +1.0f * width, -1.0f * height, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
      +1.0f * width, +1.0f * height, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
      -1.0f * width, +1.0f * height, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f
    };
    uint32_t indices[] = { 0, 1, 2, 0, 2, 3 };
    // set vertex and index data
    MeshPtr mesh = model->createMesh();
    mesh->setVertexData(vertices, 4, AT_POSITION | AT_NORMAL | AT_TEXCOORD0);
    mesh->setIndexData(indices, 6);
    // return model
    return model;
  }

  ModelPtr Root::createPlane(const string &name, float width, float height, float uTile, float vTile) {
    ModelPtr model = Root::instance()->createModel(name);
    // calculate vertex and index data
    float vertices[] = {
      -1.0f * width, 0.0f, +1.0f * height, 0.0f, 1.0f, 0.0f, -1.0f * uTile, -1.0f * vTile,
      +1.0f * width, 0.0f, +1.0f * height, 0.0f, 1.0f, 0.0f, +1.0f * uTile, -1.0f * vTile,
      +1.0f * width, 0.0f, -1.0f * height, 0.0f, 1.0f, 0.0f, +1.0f * uTile, +1.0f * vTile,
      -1.0f * width, 0.0f, -1.0f * height, 0.0f, 1.0f, 0.0f, -1.0f * uTile, +1.0f * vTile
    };
    uint32_t indices[] = { 0, 1, 2, 0, 2, 3 };
    // set vertex and index data
    MeshPtr mesh = model->createMesh();
    mesh->setVertexData(vertices, 4, AT_POSITION | AT_NORMAL | AT_TEXCOORD0);
    mesh->setIndexData(indices, 6);
    // return model
    return model;
  }

  ModelPtr Root::createCube(const string &name, float width, float height, float depth) {
    ModelPtr model = Root::instance()->createModel(name);
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
    uint32_t indices[] = {
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
    MeshPtr mesh = model->createMesh();
    mesh->setVertexData(vertices, 24, AT_POSITION | AT_NORMAL | AT_TEXCOORD0);
    mesh->setIndexData(indices, 36);
    // return model
    return model;
  }

  ModelPtr Root::createCone(const string &name, float radius, float height, uint slices, uint stacks) {
    ModelPtr model = Root::instance()->createModel(name);
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
    MeshPtr mesh = model->createMesh();
    mesh->setVertexData(vertices, vertexCount, AT_POSITION | AT_NORMAL | AT_TEXCOORD0);
    mesh->setIndexData(indices, indexCount);
    // clean up
    delete[] vertices;
    delete[] indices;
    // return model
    return model;
  }

  ModelPtr Root::createSphere(const string &name, float radius, uint slices, uint stacks) {
    ModelPtr model = Root::instance()->createModel(name);
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
    MeshPtr mesh = model->createMesh();
    mesh->setVertexData(vertices, vertexCount, AT_POSITION | AT_NORMAL | AT_TEXCOORD0);
    mesh->setIndexData(indices, indexCount);
    // clean up
    delete[] vertices;
    delete[] indices;
    // return model
    return model;
  }

  void Root::save(const string &name, const string &path) {
    // find mesmodel
    if (d->models.count(name) == 0)
      return;
    ModelPtr model = d->models[name];
    // create output stream
    OutputStream out(path);
    // write magic bytes
    out << 'S' << 'G' << 'L' << 'M';
    // write version info
    out << uint8_t(1) << uint8_t(0);
    // write mesh count
    out << uint16_t(model->meshes().size());
    // write meshes
    for (MeshPtr mesh: model->meshes()) {
      // write material info
      out << mesh->material();
      // write vertex description
      out << mesh->vertexFormat() << mesh->vertexSize() << mesh->vertexCount();
      // write vertex data
      out.write(mesh->vertexData(), mesh->vertexSize() * mesh->vertexCount());
      // write index description
      out << mesh->indexSize() << mesh->indexCount();
      // write index data
      out.write(mesh->indexData(), mesh->indexSize() * mesh->indexCount());
    }
    // write bone count
    out << uint16_t(model->bones().size());
    // write bones
    for (BonePtr bone: model->bones()) {
      // write bone name
      out << bone->name();
      // write parent name
      int boneIndex = -1;
      if (bone->parent())
        for (uint i = 0; i < model->bones().size(); ++i)
          if (model->bones().at(i).get() == bone->parent())
            boneIndex = i;
      out << uint16_t(boneIndex);
      // write transform and offset matrices
      out << bone->transform() << bone->offsetMatrix();
    }
    // write animation count
    out << uint16_t(model->animations().size());
    // write animations
    for (AnimationPtr animation: model->animations()) {
      // write name and duration
      out << animation->name() << animation->duration();
      // write track count
      out << uint16_t(animation->tracks().size());
      // write tracks
      for (AnimationTrackPtr track: animation->tracks()) {
        // write track name
        out << track->name();
        // write position key count
        out << uint16_t(track->positionKeys().size());
        // write position keys
        for (auto key: track->positionKeys())
          out << key.first << key.second;
        // write orientation key count
        out << uint16_t(track->orientationKeys().size());
        // write orientation keys
        for (auto key: track->orientationKeys())
          out << key.first << key.second;
        // write scale key count
        out << uint16_t(track->scaleKeys().size());
        // write scale keys
        for (auto key: track->scaleKeys())
          out << key.first << key.second;
      }
    }
  }

  void Root::load(const string &name, const string &path) {
    // create a new model
    ModelPtr model = Root::instance()->createModel(name);
    // create input stream
    InputStream in(path);
    // read magic bytes
    char magic[4] = { 0 };
    in >> magic[0] >> magic[1] >> magic[2] >> magic[3];
    // read version info
    uint8_t major = 0, minor = 0;
    in >> major >> minor;
    // read mesh count
    uint16_t meshCount = 0;
    in >> meshCount;
    // read meshes
    for (uint i = 0; i < meshCount; ++i) {
      // read material info
      string material;
      in >> material;
      // read vertex description
      uint16_t vertexFormat = 0, vertexSize = 0;
      uint32_t vertexCount = 0;
      in >> vertexFormat >> vertexSize >> vertexCount;
      // read vertex data
      char *vertexData = new char[vertexSize * vertexCount];
      in.read(vertexData, vertexSize * vertexCount);
      // read index description
      uint16_t indexSize = 0;
      uint32_t indexCount = 0;
      in >> indexSize >> indexCount;
      // read index data
      char *indexData = new char[indexSize * indexCount];
      in.read(indexData, indexSize * indexCount);
      // create mesh
      MeshPtr mesh = model->createMesh();
      mesh->setMaterial(material);
      mesh->setVertexData((float *)vertexData, vertexCount, vertexFormat);
      mesh->setIndexData((uint32_t *)indexData, indexCount);
      // clean up
      delete vertexData;
      delete indexData;
    }
    // read bone count
    uint16_t boneCount = 0;
    in >> boneCount;
    // read bones
    for (uint i = 0; i < boneCount; ++i) {
      // read name
      string name;
      in >> name;
      // read parent index
      uint16_t parentIndex = 0;
      in >> parentIndex;
      // read transform and offset matrices
      Matrix4f transform, offset;
      in >> transform >> offset;
      // create bone
      BonePtr parent = (parentIndex < model->bones().size()) ? model->bones().at(parentIndex) : nullptr;
      BonePtr bone = model->createBone(name);
      if (parent)
        parent->attach(bone);
      bone->setTransform(transform);
      bone->setOffsetMatrix(offset);
    }
    // read animation count
    uint16_t animationCount = 0;
    in >> animationCount;
    for (uint i = 0; i < animationCount; ++i) {
      // read name
      string name;
      in >> name;
      // read duration
      long duration = 0;
      in >> duration;
      // create animation
      AnimationPtr animation = model->createAnimation(name);
      animation->setDuration(duration);
      // read track count
      uint16_t trackCount = 0;
      in >> trackCount;
      // read tracks
      for (uint j = 0; j < trackCount; ++j) {
        // read name
        string name;
        in >> name;
        // create track
        AnimationTrackPtr track = animation->createTrack(name);
        // read position key count
        uint16_t positionKeyCount = 0;
        in >> positionKeyCount;
        // read position keys
        for (uint k = 0; k < positionKeyCount; ++k) {
          long time;
          Vector3f value;
          in >> time >> value;
          track->createPositionKey(time, value);
        }
        // read orientation key count
        uint16_t orientationKeyCount = 0;
        in >> orientationKeyCount;
        // read orientation keys
        for (uint k = 0; k < orientationKeyCount; ++k) {
          long time;
          Quaternion value;
          in >> time >> value;
          track->createOrientationKey(time, value);
        }
        // read scale key count
        uint16_t scaleKeyCount = 0;
        in >> scaleKeyCount;
        // read scale keys
        for (uint k = 0; k < scaleKeyCount; ++k) {
          long time;
          Vector3f value;
          in >> time >> value;
          track->createScaleKey(time, value);
        }
      }
    }
  }

  InstancePtr Root::createInstance(const string &model, const string &material) {
    InstancePtr instance { new Instance(model, material) };
    // add to list
    d->instances.push_back(instance);
    // return instance
    return instance;
  }

  void Root::loadScript(const string &path) {
    ifstream in(path);
    if (!in.is_open())
      return;
    // extract directory
    string directory = path.substr(0, path.find_last_of("/"));
    // parse the script
    do {
      string type, name, buffer;
      in >> type >> name;
      if (type == "program") {
        ProgramPtr program = Root::instance()->createProgram(name);
        do {
          // read next token
          in >> buffer;
          // handle token
          if (buffer == "{") {
            // do nothing
          } else if (buffer == "vertex_shader") {
            // get program path
            in >> buffer;
            // add vertex program
            if (!program->loadShaderFromPath(ST_VERTEX, directory + "/" + buffer))
              cerr << program->log() << endl;
          } else if (buffer == "fragment_shader") {
            // get program path
            in >> buffer;
            // add fragment program
            if (!program->loadShaderFromPath(ST_FRAGMENT, directory + "/" + buffer))
              cerr << program->log() << endl;
          } else if (buffer == "uniform") {
            // get uniform name
            in >> buffer;
            // add uniform
            program->addUniform(buffer);
          } else if (buffer == "}") {
            break;
          }
        } while (!in.eof());
        // link the program
        if (!program->link())
          cerr << program->log() << endl;
      } else if (type == "material") {
        MaterialPtr material = Root::instance()->createMaterial(name);
        do {
          // read next token
          in >> buffer;
          // handle token
          if (buffer == "{") {
            // do nothing
          } else if (buffer == "program") {
            // get program name
            in >> buffer;
            // set program name
            material->setProgram(buffer);
          } else if (buffer == "texture") {
            // read texture path
            in >> buffer;
            // add texture
            material->addTexture(directory + "/" + buffer);
          } else if (buffer == "cull_face") {
            // read value
            in >> buffer;
            // set face culling
            if (buffer == "none")
              material->setCullFace(CF_NONE);
            else if (buffer == "front")
              material->setCullFace(CF_FRONT);
            else if (buffer == "back")
              material->setCullFace(CF_BACK);
           } else if (buffer == "}") {
            break;
          }
        } while (!in.eof());
      }
    } while (!in.eof());

    in.close();
  }

  void Root::prepareRender(long elapsed) {
    // update fps
    d->fpsCount++;
    d->fpsTime += elapsed;
    // update animations
    for (auto it: d->models)
      it.second->updateAnimations(elapsed);
    // process nodes
    queue<SceneNodePtr> processQueue;
    // add root node to the updated Nodes
    processQueue.push(d->rootSceneNode);
    // update nodes
    while (!processQueue.empty()) {
      SceneNodePtr node = processQueue.front();
      processQueue.pop();
      // process node
      node->updateWorldTransform();
      // add instances to the queue
      for (SceneObjectPtr object: node->attachedObjects()) {
        if (object->type() == "Instance") {
          InstancePtr instance = static_pointer_cast<Instance>(object);
          if (!instance)
            continue;
          ModelPtr model = Root::instance()->retrieveModel(instance->model());
          if (!model)
            continue;
          for (MeshPtr mesh: model->meshes()) {
            string material = "Default";
            if (Root::instance()->retrieveMaterial(instance->material()))
              material = instance->material();
            else if (Root::instance()->retrieveMaterial(mesh->material()))
              material = mesh->material();
            d->renderQueue.push_back(RenderOp(mesh, material, node->worldTransform(), model->bones().size(), model->boneTransforms()));
          }
        }
      }
      // queue child nodes for processing
      for (SceneNodePtr childNode: node->attachedNodes())
        processQueue.push(childNode);
    }
    // sort the render queue
    sort(d->renderQueue.begin(), d->renderQueue.end(), [] (const RenderOp &p1, const RenderOp &p2) { return p1.material < p2.material; });
  }

  void Root::renderScene(CameraPtr camera) {
    if (!camera)
      return;
    // cache viewProjMatrix
    Matrix4f viewProjMatrix = camera->projectionMatrix() * camera->viewMatrix();
    // render scene
    uint i = 0;
    while (i < d->renderQueue.size()) {
      // get material
      MaterialPtr material = Root::instance()->retrieveMaterial(d->renderQueue.at(i).material);
      if (!material)
        continue;
      // get program
      ProgramPtr program = Root::instance()->retrieveProgram(material->program());
      if (!program)
        continue;
      // bind the material
      material->bind();
      // render meshes
      do {
        MeshPtr mesh = d->renderQueue.at(i).mesh;
        Matrix4f transform = d->renderQueue.at(i).transform;
        uint numBones = d->renderQueue.at(i).numBones;
        const float *transforms = d->renderQueue.at(i).boneTransforms;
        // set uniforms
        program->setUniform("modelMatrix", transform);
        program->setUniform("modelViewProjMatrix", viewProjMatrix * transform);
        if (numBones)
          program->setUniform4fv("boneTransforms", numBones, transforms);
        // render the mesh
        mesh->render(camera);
        // proceed to the next item
        i++;
      } while (i < d->renderQueue.size() && d->renderQueue.at(i).material == material->name());
    }
    // clear render queue
    d->renderQueue.clear();
  }

  void Root::renderLights(CameraPtr camera, Vector2f viewportSize) {
    if (!camera)
      return;
    // render lights
    for (const string type: { "Light/Directional", "Light/Point", "Light/Spot" }) {
      ProgramPtr program = Root::instance()->retrieveProgram(type);
      if (!program)
        continue;
      // bine the program
      program->bind();
      // set parameters
      program->setUniform("texture0", 0);
      program->setUniform("texture1", 1);
      program->setUniform("texture2", 2);
      program->setUniform("viewportSize", viewportSize);
      program->setUniform("cameraPos", camera->parent()->worldPosition());
      // render lights
      for (LightPtr light: d->lights)
        if (light->type() == type)
          light->render(camera);
    }
  }

  const float Root::fps() const {
    if (d->fpsTime >= 1000) {
      d->fps = d->fpsCount / (d->fpsTime * 0.001f);
      d->fpsCount = 0;
      d->fpsTime = 0;
    }
    // return fps
    return d->fps;
  }
}
