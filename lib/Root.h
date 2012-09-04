#ifndef ROOT_H
#define ROOT_H

#include "Types.h"

namespace SimpleGL {
  class RootPrivate;

  void initialize();
  void destroy();

  class Root {
  public:
    Root();
    ~Root();

    static Root *instance();

    WindowPtr createWindow(int width = 800, int height = 600);

    SceneNodePtr createSceneNode();
    SceneNodePtr rootSceneNode() const;

    LightPtr createLight(const string &type);

    CameraPtr createCamera(const string &name);
    CameraPtr retrieveCamera(const string &name);
    void removeCamera(const string &name);

    ProgramPtr createProgram(const string &name);
    ProgramPtr retrieveProgram(const string &name);
    void removeProgram(const string &name);

    MaterialPtr createMaterial(const string &name);
    MaterialPtr retrieveMaterial(const string &name);
    void removeMaterial(const string &name);

    ModelPtr createModel(const string &name);
    ModelPtr retrieveModel(const string &name);
    void removeModel(const string &name);

    ModelPtr createQuad(const string &name, float width = 1.0f, float height = 1.0f);
    ModelPtr createPlane(const string &name, float width = 1.0f, float height = 1.0f, float uTile = 1.0f, float vTile = 1.0f);
    ModelPtr createCube(const string &name, float width = 1.0f, float height = 1.0f, float depth = 1.0f);
    ModelPtr createCone(const string &name, float radius = 1.0f, float height = 1.0f, uint slices = 16, uint stacks = 16);
    ModelPtr createSphere(const string &name, float radius = 1.0f, uint slices = 16, uint stacks = 16);
    void save(const string &name, const string &path);
    void load(const string &name, const string &path);

    InstancePtr createInstance(const string &model, const string &material);

    void prepareRender(long elapsed);
    void renderScene(CameraPtr camera);
    void renderLights(CameraPtr camera, Vector2f viewportSize);

    const float fps() const;
  private:
    RootPrivate *d;
  };
}

#endif // ROOT_H
