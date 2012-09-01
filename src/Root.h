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
  class Viewport;
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
    const vector<Window *> &windows() const;

    SceneNode *createSceneNode();
    SceneNode *rootSceneNode() const;
    const vector<SceneNode *> &sceneNodes() const;

    Light *createLight(LightType type);
    const vector<Light *> &lights() const;

    Camera *createCamera();
    const vector<Camera *> &cameras() const;

    Program *createProgram(const string &name);
    Program *retrieveProgram(const string &name);
    const vector<Program *> &programs() const;

    Material *createMaterial(const string &name);
    Material *retrieveMaterial(const string &name);
    const vector<Material *> &materials() const;

    Mesh *createMesh(const string &name);
    Mesh *retrieveMesh(const string &name);
    Mesh *createQuad(const string &name, float width = 1.0f, float height = 1.0f);
    Mesh *createPlane(const string &name, float width = 1.0f, float height = 1.0f, float uTile = 1.0f, float vTile = 1.0f);
    Mesh *createCube(const string &name, float width = 1.0f, float height = 1.0f, float depth = 1.0f);
    Mesh *createCone(const string &name, float radius = 1.0f, float height = 1.0f, uint slices = 16, uint stacks = 16);
    Mesh *createSphere(const string &name, float radius = 1.0f, uint slices = 16, uint stacks = 16);
    void save(const string &name, const string &path);
    void load(const string &name, const string &path);
    const vector<Mesh *> &meshes() const;

    Instance *createInstance(const string &mesh, const string &material);
    const vector<Instance *> &instances() const;

    void prepareRender(long time);
    void renderScene(Window *window, Viewport *viewport);
    void renderLights(Window *window, Viewport *viewport);

    const float fps() const;
  private:
    RootPrivate *d;
  };
}

#endif // ROOT_H
