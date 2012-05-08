#ifndef INSTANCE_H
#define INSTANCE_H

#include "Renderable.h"
#include "SceneObject.h"

namespace SimpleGL {
  class InstancePrivate;

  class Instance : public Renderable, public SceneObject {
  public:
    Instance(const String &name);
    ~Instance();

    const String &name() const;

    const String &mesh() const;
    void setMesh(const String &mesh);

    void render(Camera *camera);

  private:
    InstancePrivate *d;
  };
}

#endif // INSTANCE_H
