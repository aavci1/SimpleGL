#include "SceneObject.h"

namespace SimpleGL {
  class SceneObjectPrivate {
  public:
    SceneObjectPrivate() {
    }

    ~SceneObjectPrivate() {
    }

    shared_ptr<SceneNode> parent { nullptr };
  };

  SceneObject::SceneObject() : d(new SceneObjectPrivate()) {
  }

  SceneObject::~SceneObject() {
    delete d;
  }

  shared_ptr<SceneNode> SceneObject::parent() const {
    return d->parent;
  }

  void SceneObject::setParent(shared_ptr<SceneNode> parent) {
    d->parent = parent;
  }
}
