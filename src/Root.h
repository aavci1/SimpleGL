#ifndef ROOT_H
#define ROOT_H

#include "Types.h"

#include <vector>

namespace SimpleGL {
  class RootPrivate;
  class Camera;
  class Instance;
  class Light;
  class Material;
  class Mesh;
  class Program;
  class SceneNode;
  class Texture;
  class Window;

  class Root {
  protected:
    Root();
    ~Root();

  public:
    static void initialize();
    static void destroy();
    static Root *instance();

    Window *createWindow(int width = 800, int height = 600);
    const std::vector<Window *> &windows() const;

    SceneNode *createSceneNode();
    SceneNode *rootSceneNode() const;
    const std::vector<SceneNode *> &sceneNodes() const;

    Light *createLight(LightType type);
    const std::vector<Light *> &lights() const;

    Camera *createCamera();
    const std::vector<Camera *> &cameras() const;

    Texture *createTexture(const String &name, const String &path);
    Texture *retrieveTexture(const String &name);
    const std::vector<Texture *> &textures() const;

    Program *createProgram(const String &name);
    Program *retrieveProgram(const String &name);
    const std::vector<Program *> &programs() const;

    Material *createMaterial(const String &name);
    Material *retrieveMaterial(const String &name);
    const std::vector<Material *> &materials() const;

    Mesh *createMesh(const String &name);
    Mesh *retrieveMesh(const String &name);
    Mesh *createQuad(const String &name, float width = 1.0f, float height = 1.0f);
    Mesh *createPlane(const String &name, float width = 1.0f, float height = 1.0f, float uTile = 1.0f, float vTile = 1.0f);
    Mesh *createCube(const String &name, float width = 1.0f, float height = 1.0f, float depth = 1.0f);
    Mesh *createCone(const String &name, float radius = 1.0f, float height = 1.0f, uint slices = 16, uint stacks = 16);
    Mesh *createSphere(const String &name, float radius = 1.0f, uint slices = 16, uint stacks = 16);
    Mesh *loadMesh(const String &name, const String &path);
    const std::vector<Mesh *> &meshes() const;

    Instance *createInstance(const String &mesh, const String &material);
    const std::vector<Instance *> &instances() const;

    const long renderOneFrame(long time);

    const float fps() const;
  private:
    RootPrivate *d;
  };
}

#endif // ROOT_H
