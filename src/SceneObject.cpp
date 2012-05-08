#include "SceneObject.h"

namespace SimpleGL {
  class SceneObjectPrivate {
  public:
    SceneObjectPrivate() : parentSceneNode(0) {
    }

    ~SceneObjectPrivate() {
    }

    SceneNode *parentSceneNode;
  };

  SceneObject::SceneObject() : d(new SceneObjectPrivate()) {
  }

  SceneObject::~SceneObject() {
    delete d;
  }

  SceneNode *SceneObject::parentSceneNode() const {
    return d->parentSceneNode;
  }

  void SceneObject::setParentSceneNode(SceneNode *node) {
    d->parentSceneNode = node;
  }
}
