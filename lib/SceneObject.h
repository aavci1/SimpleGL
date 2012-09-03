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

    SceneNodePtr parent() const;
    void setParent(SceneNodePtr parent);

  private:
    SceneObjectPrivate *d;
  };
}

#endif // SCENEOBJECT_H
