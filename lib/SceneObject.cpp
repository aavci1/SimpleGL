#include "SceneObject.h"

namespace SimpleGL {
  class SceneObjectPrivate {
  public:
    SceneObjectPrivate() {
    }

    ~SceneObjectPrivate() {
    }

    SceneNode *parent { nullptr };
  };

  SceneObject::SceneObject() : d(new SceneObjectPrivate()) {
  }

  SceneObject::~SceneObject() {
    delete d;
  }

  SceneNode *SceneObject::parent() const {
    return d->parent;
  }

  void SceneObject::setParent(SceneNode *parent) {
    d->parent = parent;
  }
}
