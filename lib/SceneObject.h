#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

namespace SimpleGL {
  class SceneObjectPrivate;
  class SceneNode;

  class SceneObject {
  public:
    SceneObject();
    virtual ~SceneObject();

    SceneNode *parentSceneNode() const;
    void setParentSceneNode(SceneNode *node);

  private:
    SceneObjectPrivate *d;
  };
}

#endif // SCENEOBJECT_H
