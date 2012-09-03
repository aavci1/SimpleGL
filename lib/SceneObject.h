#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "Types.h"

namespace SimpleGL {
  class SceneObjectPrivate;
  class SceneNode;

  class SceneObject {
  public:
    SceneObject();
    virtual ~SceneObject();

    virtual string type() const = 0;

    shared_ptr<SceneNode> parent() const;
    void setParent(shared_ptr<SceneNode> parent);

  private:
    SceneObjectPrivate *d;
  };
}

#endif // SCENEOBJECT_H
