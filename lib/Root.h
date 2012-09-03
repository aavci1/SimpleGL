#ifndef ROOT_H
#define ROOT_H

#include "Types.h"

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

  void initialize();
  void destroy();

  class Root {
  public:
    Root();
    ~Root();

    static Root *instance();

    shared_ptr<Window> createWindow(int width = 800, int height = 600);
    // const vector<shared_ptr<Window>> &windows() const;

    shared_ptr<SceneNode> createSceneNode();
    shared_ptr<SceneNode> rootSceneNode() const;
    // const vector<shared_ptr<SceneNode>> &sceneNodes() const;

    shared_ptr<Light> createLight(string type);
    const vector<shared_ptr<Light>> &lights() const;

    shared_ptr<Camera> createCamera();
    // const vector<Camera *> &cameras() const;

    shared_ptr<Program> createProgram(const string &name);
    shared_ptr<Program> retrieveProgram(const string &name);
    // const vector<Program *> &programs() const;

    shared_ptr<Material> createMaterial(const string &name);
    shared_ptr<Material> retrieveMaterial(const string &name);
    const vector<shared_ptr<Material>> &materials() const;

    shared_ptr<Mesh> createMesh(const string &name);
    shared_ptr<Mesh> retrieveMesh(const string &name);
    shared_ptr<Mesh> createQuad(const string &name, float width = 1.0f, float height = 1.0f);
    shared_ptr<Mesh> createPlane(const string &name, float width = 1.0f, float height = 1.0f, float uTile = 1.0f, float vTile = 1.0f);
    shared_ptr<Mesh> createCube(const string &name, float width = 1.0f, float height = 1.0f, float depth = 1.0f);
    shared_ptr<Mesh> createCone(const string &name, float radius = 1.0f, float height = 1.0f, uint slices = 16, uint stacks = 16);
    shared_ptr<Mesh> createSphere(const string &name, float radius = 1.0f, uint slices = 16, uint stacks = 16);
    void save(const string &name, const string &path);
    void load(const string &name, const string &path);
    // const vector<shared_ptr<Mesh> > &meshes() const;

    shared_ptr<Instance> createInstance(const string &mesh, const string &material);
    // const vector<shared_ptr<Instance>> &instances() const;

    void prepareRender(long elapsed);
    void renderScene(Window *window, Viewport *viewport);
    void renderLights(Window *window, Viewport *viewport);

    const float fps() const;
  private:
    RootPrivate *d;
  };
}

#endif // ROOT_H
