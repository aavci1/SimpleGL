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
#include "OutputStream.h"
#include "PointLight.h"
#include "Program.h"
#include "Root.h"
#include "SceneNode.h"
#include "SpotLight.h"
#include "SubMesh.h"
#include "Viewport.h"
#include "Window.h"

#include <GL/glew.h>

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
    vector<CameraPtr> cameras;
    map<string, MeshPtr> meshes;
    map<string, MaterialPtr> materials;
    map<string, ProgramPtr> programs;

    Vector2i mousePosition { 0, 0 };
    long animationTime { 0 };

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

  LightPtr Root::createLight(string type) {
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

  CameraPtr Root::createCamera() {
    CameraPtr camera { new Camera() };
    // add to list
    d->cameras.push_back(camera);
    // return camera
    return camera;
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

  MeshPtr Root::createMesh(const string &name) {
    MeshPtr mesh { new Mesh(name) };
    // add to list
    if (!name.empty())
      d->meshes[name] = mesh;
    // return mesh
    return mesh;
  }

  MeshPtr Root::retrieveMesh(const string &name) {
    if (name.empty() || d->meshes.count(name) == 0)
      return nullptr;
    return d->meshes[name];
  }

  MeshPtr Root::createQuad(const string &name, float width, float height) {
    MeshPtr mesh = Root::instance()->createMesh(name);
    // calculate vertex and index data
    float vertices[] = {
      -1.0f * width, -1.0f * height, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
      +1.0f * width, -1.0f * height, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
      +1.0f * width, +1.0f * height, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
      -1.0f * width, +1.0f * height, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f
    };
    uint32_t indices[] = { 0, 1, 2, 0, 2, 3 };
    // set vertex and index data
    SubMeshPtr subMesh = mesh->createSubMesh();
    subMesh->setVertexData(vertices, 4, AT_POSITION | AT_NORMAL | AT_TEXCOORD0);
    subMesh->setIndexData(indices, 6);
    // return mesh
    return mesh;
  }

  MeshPtr Root::createPlane(const string &name, float width, float height, float uTile, float vTile) {
    MeshPtr mesh = Root::instance()->createMesh(name);
    // calculate vertex and index data
    float vertices[] = {
      -1.0f * width, 0.0f, +1.0f * height, 0.0f, 1.0f, 0.0f, -1.0f * uTile, -1.0f * vTile,
      +1.0f * width, 0.0f, +1.0f * height, 0.0f, 1.0f, 0.0f, +1.0f * uTile, -1.0f * vTile,
      +1.0f * width, 0.0f, -1.0f * height, 0.0f, 1.0f, 0.0f, +1.0f * uTile, +1.0f * vTile,
      -1.0f * width, 0.0f, -1.0f * height, 0.0f, 1.0f, 0.0f, -1.0f * uTile, +1.0f * vTile
    };
    uint32_t indices[] = { 0, 1, 2, 0, 2, 3 };
    // set vertex and index data
    SubMeshPtr subMesh = mesh->createSubMesh();
    subMesh->setVertexData(vertices, 4, AT_POSITION | AT_NORMAL | AT_TEXCOORD0);
    subMesh->setIndexData(indices, 6);
    // return mesh
    return mesh;
  }

  MeshPtr Root::createCube(const string &name, float width, float height, float depth) {
    MeshPtr mesh = Root::instance()->createMesh(name);
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
    SubMeshPtr subMesh = mesh->createSubMesh();
    subMesh->setVertexData(vertices, 24, AT_POSITION | AT_NORMAL | AT_TEXCOORD0);
    subMesh->setIndexData(indices, 36);
    // return mesh
    return mesh;
  }

  MeshPtr Root::createCone(const string &name, float radius, float height, uint slices, uint stacks) {
    MeshPtr mesh = Root::instance()->createMesh(name);
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
    SubMeshPtr subMesh = mesh->createSubMesh();
    subMesh->setVertexData(vertices, vertexCount, AT_POSITION | AT_NORMAL | AT_TEXCOORD0);
    subMesh->setIndexData(indices, indexCount);
    // clean up
    delete[] vertices;
    delete[] indices;
    // return mesh
    return mesh;
  }

  MeshPtr Root::createSphere(const string &name, float radius, uint slices, uint stacks) {
    MeshPtr mesh = Root::instance()->createMesh(name);
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
    SubMeshPtr subMesh = mesh->createSubMesh();
    subMesh->setVertexData(vertices, vertexCount, AT_POSITION | AT_NORMAL | AT_TEXCOORD0);
    subMesh->setIndexData(indices, indexCount);
    // clean up
    delete[] vertices;
    delete[] indices;
    // return mesh
    return mesh;
  }

  void Root::save(const string &name, const string &path) {
    // find mesh
    if (d->meshes.count(name) == 0)
      return;
    MeshPtr mesh = d->meshes[name];
    // create output stream
    OutputStream out(path);
    // write magic bytes
    out << 'S' << 'G' << 'L' << 'M';
    // write version info
    out << uint8_t(1) << uint8_t(0);
    // write submesh count
    out << uint16_t(mesh->subMeshes().size());
    // write submeshes
    for (SubMeshPtr subMesh: mesh->subMeshes()) {
      // write material info
      out << subMesh->material();
      // write vertex description
      out << subMesh->vertexFormat() << subMesh->vertexSize() << subMesh->vertexCount();
      // write vertex data
      out.write(subMesh->vertexData(), subMesh->vertexSize() * subMesh->vertexCount());
      // write index description
      out << subMesh->indexSize() << subMesh->indexCount();
      // write index data
      out.write(subMesh->indexData(), subMesh->indexSize() * subMesh->indexCount());
    }
    // write bone count
    out << uint16_t(mesh->bones().size());
    // write bones
    for (BonePtr bone: mesh->bones()) {
      // write bone name
      out << bone->name();
      // write parent name
      int boneIndex = -1;
      if (bone->parent())
        for (uint i = 0; i < mesh->bones().size(); ++i)
          if (mesh->bones().at(i) == bone->parent())
            boneIndex = i;
      out << uint16_t(boneIndex);
      // write transform and offset matrices
      out << bone->transform() << bone->offsetMatrix();
    }
    // write animation count
    out << uint16_t(mesh->animations().size());
    // write animations
    for (AnimationPtr animation: mesh->animations()) {
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
    // create a new mesh
    MeshPtr mesh = Root::instance()->createMesh(name);
    // create input stream
    InputStream in(path);
    // read magic bytes
    char magic[4] = { 0 };
    in >> magic[0] >> magic[1] >> magic[2] >> magic[3];
    // read version info
    uint8_t major = 0, minor = 0;
    in >> major >> minor;
    // read submesh count
    uint16_t subMeshCount = 0;
    in >> subMeshCount;
    // read subMeshes
    for (uint i = 0; i < subMeshCount; ++i) {
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
      // create submesh
      SubMeshPtr subMesh = mesh->createSubMesh();
      subMesh->setMaterial(material);
      subMesh->setVertexData((float *)vertexData, vertexCount, vertexFormat);
      subMesh->setIndexData((uint32_t *)indexData, indexCount);
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
      BonePtr parent = (parentIndex < mesh->bones().size()) ? mesh->bones().at(parentIndex) : nullptr;
      BonePtr bone = mesh->createBone(name);
      bone->setParent(parent);
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
      AnimationPtr animation = mesh->createAnimation(name);
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

  InstancePtr Root::createInstance(const string &mesh, const string &material) {
    InstancePtr instance { new Instance(mesh, material) };
    // add to list
    d->instances.push_back(instance);
    // return instance
    return instance;
  }

  void Root::prepareRender(long elapsed) {
    // update fps
    d->fpsCount++;
    d->fpsTime += elapsed;
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
      // update animations
      for (InstancePtr instance: d->instances) {
        if (instance->parent() != node)
          continue;
        MeshPtr mesh = Root::instance()->retrieveMesh(instance->mesh());
        if (!mesh)
          continue;
        // update each animation
        for (AnimationPtr animation: mesh->animations()) {
          // add time elapsed
          d->animationTime += elapsed;
          // update bone transforms
          for (AnimationTrackPtr track: animation->tracks())
            for (BonePtr bone: mesh->bones())
              if (bone->name() == track->name())
                bone->setTransform(track->transform(d->animationTime % animation->duration()));
        }
        // update skeleton
        mesh->updateBones();
      }
      // queue child nodes for processing
      for (SceneNodePtr childNode: d->sceneNodes)
        if (childNode->parent() == node)
          processQueue.push(childNode);
    }
  }

  void Root::renderScene(Window *window, Viewport *viewport) {
    if (viewport == nullptr || viewport->camera() == nullptr)
      return;
    CameraPtr camera = viewport->camera();
    // render scene
    std::queue<SceneNodePtr> processQueue;
    // add root node to the updated Nodes
    processQueue.push(d->rootSceneNode);
    // update nodes
    while (!processQueue.empty()) {
      SceneNodePtr node = processQueue.front();
      processQueue.pop();
      // render instances
      for (InstancePtr instance: d->instances) {
        if (instance->parent() != node)
          continue;
        MeshPtr mesh = Root::instance()->retrieveMesh(instance->mesh());
        if (!mesh)
          continue;
        // draw sub meshes
        for (SubMeshPtr subMesh: mesh->subMeshes()) {
          MaterialPtr material = Root::instance()->retrieveMaterial(instance->material());
          if (!material)
            material = Root::instance()->retrieveMaterial(subMesh->material());
          if (!material)
            material = Root::instance()->retrieveMaterial("Default");
          if (!material)
            continue;
          ProgramPtr program = Root::instance()->retrieveProgram(material->program());
          if (!program)
            continue;
          // bind the material
          material->bind();
          // set uniforms
          program->setUniform("ModelMatrix", node->worldTransform());
          program->setUniform("ModelViewProjMatrix", camera->projectionMatrix() * camera->viewMatrix() * node->worldTransform());
          for (uint l = 0; l < mesh->bones().size(); ++l) {
            char boneName[12] = { 0 };
            snprintf(boneName, sizeof(boneName), "Bones[%d]", l);
            program->setUniform(boneName, mesh->bones().at(l)->worldTransform() * mesh->bones().at(l)->offsetMatrix());
          }
          // render the mesh
          subMesh->render(camera);
          // unbind material
          material->unbind();
        }
      }
      // queue child nodes for processing
      for (SceneNodePtr childNode: d->sceneNodes)
        if (childNode->parent() == node)
          processQueue.push(childNode);
    }
  }

  void Root::renderLights(Window *window, Viewport *viewport) {
    if (viewport == nullptr || viewport->camera() == nullptr)
      return;
    // get camera
    CameraPtr camera = viewport->camera();
    // render lights
    for (LightPtr light: d->lights) {
      ProgramPtr program = Root::instance()->retrieveProgram(light->type());
      if (!program)
        continue;
      // bine the program
      program->bind();
      // set parameters
      program->setUniform("texture0", 0);
      program->setUniform("texture1", 1);
      program->setUniform("texture2", 2);
      program->setUniform("viewportSize", Vector2f(viewport->width() * window->width(), viewport->height() * window->height()));
      program->setUniform("cameraPos", camera->parent()->worldPosition());
      // render the light
      light->render(camera);
      // unbind the program
      program->unbind();
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
