#include "SceneObject.h"

namespace SimpleGL {
  class SceneObjectPrivate {
  public:
    SceneObjectPrivate() {
    }

    ~SceneObjectPrivate() {
    }

    SceneNodePtr parent { nullptr };
  };

  SceneObject::SceneObject() : d(new SceneObjectPrivate()) {
  }

  SceneObject::~SceneObject() {
    delete d;
  }

  SceneNodePtr SceneObject::parent() const {
    return d->parent;
  }

  void SceneObject::setParent(SceneNodePtr parent) {
    d->parent = parent;
  }
}
