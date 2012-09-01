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
#include "Texture.h"
#include "Viewport.h"
#include "Window.h"

#include <GL/glew.h>

namespace SimpleGL {
  static Root *_instance { nullptr };

  class RootPrivate {
  public:
    RootPrivate() {
    }

    ~RootPrivate() {
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

    void prepareRender(SceneNode *node, long delta) {
      // update world transform
      node->updateWorldTransform();
      // update animations
      for (SceneObject *object: node->attachedObjects()) {
        Instance *instance = dynamic_cast<Instance *>(object);
        if (!instance)
          continue;
        Mesh *mesh = Root::instance()->retrieveMesh(instance->mesh());
        if (!mesh)
          continue;
        // update each animation
        for (Animation *animation: mesh->animations()) {
          // add time delta
          animationTime += delta;
          // update bone transforms
          for (AnimationTrack *track: animation->tracks())
            for (Bone *bone: mesh->bones())
              if (bone->name() == track->name())
                bone->setTransform(track->transform(animationTime % animation->duration()));
        }
        // update bones world transforms
        if (mesh->bones().size())
          mesh->bones().at(0)->updateWorldTransform();
      }
      // visit child nodes
      for (SceneNode *childNode: node->childNodes())
        prepareRender(childNode, delta);
    }

    void renderScene(Camera *camera, SceneNode *node) {
      // visit child nodes
      for (SceneNode *childNode: node->childNodes())
        renderScene(camera, childNode);
      // render meshes
      for (SceneObject *object: node->attachedObjects()) {
        Instance *instance = dynamic_cast<Instance *>(object);
        if (!instance)
          continue;
        Mesh *mesh = Root::instance()->retrieveMesh(instance->mesh());
        if (!mesh)
          continue;
        // draw sub meshes
        for (SubMesh *subMesh: mesh->subMeshes()) {
          Material *material = Root::instance()->retrieveMaterial(instance->material());
          if (!material)
            material = Root::instance()->retrieveMaterial(subMesh->material());
          if (!material)
            continue;
          Program *program = Root::instance()->retrieveProgram(material->program());
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
    }

    vector<Window *> windows;
    vector<SceneNode *> sceneNodes;
    vector<Light *> lights;
    vector<Camera *> cameras;
    vector<Texture *> textures;
    vector<Program *> programs;
    vector<Material *> materials;
    vector<Mesh *> meshes;
    vector<Instance *> instances;

    Vector2i mousePosition { 0, 0 };
    long time { 0 };
    long animationTime { 0 };

    long fpsTime { 0 };
    long fpsCount { 0 };
    float fps { 0.0f };
  };

  Root::Root() : d(new RootPrivate()) {
    // create root scene node
    d->sceneNodes.push_back(new SceneNode());
    // initialize glew
    glewInit();
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

  const vector<Window *> &Root::windows() const {
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

  const vector<SceneNode *> &Root::sceneNodes() const {
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

  const vector<Light *> &Root::lights() const {
    return d->lights;
  }

  Camera *Root::createCamera() {
    Camera *camera = new Camera();
    // add to list
    d->cameras.push_back(camera);
    // return camera
    return camera;
  }

  const vector<Camera *> &Root::cameras() const {
    return d->cameras;
  }

  Texture *Root::createTexture(const string &name, const string &path) {
    cout << "Root::createTexture(\"" << name << "\", \"" << path << "\");" << endl;

    Texture *texture = new Texture(name, path);
    // add to list
    d->textures.push_back(texture);
    // return texture
    return texture;
  }

  Texture *Root::retrieveTexture(const string &name) {
    if (name == "")
      return 0;
    for (uint i = 0; i < d->textures.size(); ++i)
      if (d->textures.at(i)->name() == name)
        return d->textures.at(i);
    return 0;
  }

  const vector<Texture *> &Root::textures() const {
    return d->textures;
  }

  Program *Root::createProgram(const string &name) {
    cout << "Root::createProgram(\"" << name << "\");" << endl;

    Program *program = new Program(name);
    // add to list
    d->programs.push_back(program);
    // return program
    return program;
  }

  Program *Root::retrieveProgram(const string &name) {
    if (name == "")
      return 0;
    for (uint i = 0; i < d->programs.size(); ++i)
      if (d->programs.at(i)->name() == name)
        return d->programs.at(i);
    return 0;
  }

  const vector<Program *> &Root::programs() const {
    return d->programs;
  }

  Material *Root::createMaterial(const string &name) {
    cout << "Root::createMaterial(\"" << name << "\");" << endl;

    Material *material = new Material(name);
    // add to list
    d->materials.push_back(material);
    // return material
    return material;
  }

  Material *Root::retrieveMaterial(const string &name) {
    if (name == "")
      return 0;
    for (uint i = 0; i < d->materials.size(); ++i)
      if (d->materials.at(i)->name() == name)
        return d->materials.at(i);
    return 0;
  }

  const vector<Material *> &Root::materials() const {
    return d->materials;
  }

  Mesh *Root::createMesh(const string &name) {
    cout << "Root::createMesh(\"" << name << "\");" << endl;

    Mesh *mesh = new Mesh(name);
    // add to list
    d->meshes.push_back(mesh);
    // return mesh
    return mesh;
  }

  Mesh *Root::retrieveMesh(const string &name) {
    if (name == "")
      return 0;
    for (uint i = 0; i < d->meshes.size(); ++i)
      if (d->meshes.at(i)->name() == name)
        return d->meshes.at(i);
    return 0;
  }

  Mesh *Root::createQuad(const string &name, float width, float height) {
    Mesh *mesh = Root::instance()->createMesh(name);
    // calculate vertex and index data
    float vertices[] = {
      -1.0f * width, -1.0f * height, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
      +1.0f * width, -1.0f * height, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
      +1.0f * width, +1.0f * height, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
      -1.0f * width, +1.0f * height, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f
    };
    uint32_t indices[] = { 0, 1, 2, 0, 2, 3 };
    // set vertex and index data
    SubMesh *subMesh = mesh->createSubMesh();
    subMesh->setVertexData(vertices, 4, AT_POSITION | AT_NORMAL | AT_TEXCOORD0);
    subMesh->setIndexData(indices, 6);
    // return mesh
    return mesh;
  }

  Mesh *Root::createPlane(const string &name, float width, float height, float uTile, float vTile) {
    Mesh *mesh = Root::instance()->createMesh(name);
    // calculate vertex and index data
    float vertices[] = {
      -1.0f * width, 0.0f, +1.0f * height, 0.0f, 1.0f, 0.0f, -1.0f * uTile, -1.0f * vTile,
      +1.0f * width, 0.0f, +1.0f * height, 0.0f, 1.0f, 0.0f, +1.0f * uTile, -1.0f * vTile,
      +1.0f * width, 0.0f, -1.0f * height, 0.0f, 1.0f, 0.0f, +1.0f * uTile, +1.0f * vTile,
      -1.0f * width, 0.0f, -1.0f * height, 0.0f, 1.0f, 0.0f, -1.0f * uTile, +1.0f * vTile
    };
    uint32_t indices[] = { 0, 1, 2, 0, 2, 3 };
    // set vertex and index data
    SubMesh *subMesh = mesh->createSubMesh();
    subMesh->setVertexData(vertices, 4, AT_POSITION | AT_NORMAL | AT_TEXCOORD0);
    subMesh->setIndexData(indices, 6);
    // return mesh
    return mesh;
  }

  Mesh *Root::createCube(const string &name, float width, float height, float depth) {
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
    SubMesh *subMesh = mesh->createSubMesh();
    subMesh->setVertexData(vertices, 24, AT_POSITION | AT_NORMAL | AT_TEXCOORD0);
    subMesh->setIndexData(indices, 36);
    // return mesh
    return mesh;
  }

  Mesh *Root::createCone(const string &name, float radius, float height, uint slices, uint stacks) {
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
    SubMesh *subMesh = mesh->createSubMesh();
    subMesh->setVertexData(vertices, vertexCount, AT_POSITION | AT_NORMAL | AT_TEXCOORD0);
    subMesh->setIndexData(indices, indexCount);
    // clean up
    delete[] vertices;
    delete[] indices;
    // return mesh
    return mesh;
  }

  Mesh *Root::createSphere(const string &name, float radius, uint slices, uint stacks) {
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
    SubMesh *subMesh = mesh->createSubMesh();
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
    Mesh *mesh = nullptr;
    for (Mesh *m: d->meshes)
      if (m->name() == name)
        mesh = m;
    // check if mesh found
    if (!mesh)
      return;
    // create output stream
    OutputStream out(path);
    // write magic bytes
    out << 'S' << 'G' << 'L' << 'M';
    // write version info
    out << uint8_t(1) << uint8_t(0);
    // write submesh count
    out << uint16_t(mesh->subMeshes().size());
    // write submeshes
    for (SubMesh *subMesh: mesh->subMeshes()) {
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
    for (Bone *bone: mesh->bones()) {
      // write bone name
      out << bone->name();
      // write parent name
      int boneIndex = -1;
      if (bone->parentBone())
        for (uint i = 0; i < mesh->bones().size(); ++i)
          if (mesh->bones().at(i) == bone->parentBone())
            boneIndex = i;
      out << uint16_t(boneIndex);
      // write transform and offset matrices
      out << bone->transform() << bone->offsetMatrix();
    }
    // write animation count
    out << uint16_t(mesh->animations().size());
    // write animations
    for (Animation *animation: mesh->animations()) {
      // write name and duration
      out << animation->name() << animation->duration();
      // write track count
      out << uint16_t(animation->tracks().size());
      // write tracks
      for (AnimationTrack *track: animation->tracks()) {
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
    cout << "Root::load(\"" << name << "\", \"" << path << "\");" << endl;
    // create a new mesh
    Mesh *mesh = Root::instance()->createMesh(name);
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
      SubMesh *subMesh = mesh->createSubMesh();
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
      Bone *parentBone = (parentIndex < mesh->bones().size()) ? mesh->bones().at(parentIndex) : nullptr;
      Bone *bone = mesh->createBone(name, parentBone);
      if (parentBone)
        parentBone->attachBone(bone);
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
      Animation *animation = mesh->createAnimation(name);
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
        AnimationTrack *track = animation->createTrack(name);
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

  const vector<Mesh *> &Root::meshes() const {
    return d->meshes;
  }

  Instance *Root::createInstance(const string &mesh, const string &material) {
    Instance *instance = new Instance(mesh, material);
    // add to list
    d->instances.push_back(instance);
    // return instance
    return instance;
  }

  const vector<Instance *> &Root::instances() const {
    return d->instances;
  }

  void Root::prepareRender(long time) {
    // calculate time since last frame
    long delta = (d->time == 0) ? 0 : (time - d->time);
    d->time = time;
    // update fps
    d->fpsCount++;
    d->fpsTime += delta;
    // update world transformations and animations
    d->prepareRender(d->sceneNodes.at(0), delta);
  }

  void Root::renderScene(Window *window, Viewport *viewport) {
    if (viewport == nullptr || viewport->camera() == nullptr)
      return;
    // render scene
    d->renderScene(viewport->camera(), d->sceneNodes.at(0));
  }

  void Root::renderLights(Window *window, Viewport *viewport) {
    if (viewport == nullptr || viewport->camera() == nullptr)
      return;
    // get camera
    Camera *camera = viewport->camera();
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
      program->setUniform("viewportSize", Vector2f(viewport->width() * window->width(), viewport->height() * window->height()));
      program->setUniform("cameraPos", camera->parentSceneNode()->worldPosition());
      // render the light
      for (uint k = 0; k < d->lights.size(); ++k)
        if (d->lights.at(k)->type() == type)
          d->lights.at(k)->render(camera);
      // deselect material
      program->unbind();
    }
  }

  const float Root::fps() const {
    if (d->fpsTime >= 1000) {
      d->fps = d->fpsCount / (d->fpsTime * 0.001f);
      d->fpsCount = 0;
      d->fpsTime = 0;
      cout << "FPS: " << d->fps << endl;
    }
    // return fps
    return d->fps;
  }
}
