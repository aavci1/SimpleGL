#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "Types.h"

namespace SimpleGL {
  class SceneObjectPrivate;

  class SceneObject {
  public:
    SceneObject();
    virtual ~SceneObject();

    virtual string type() const = 0;

    SceneNode *parent() const;
    void setParent(SceneNode *parent);

  private:
    SceneObjectPrivate *d;
  };
}

#endif // SCENEOBJECT_H
