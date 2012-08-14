#ifndef INSTANCE_H
#define INSTANCE_H

#include "SceneObject.h"
#include "Types.h"

namespace SimpleGL {
  class InstancePrivate;

  class Instance : public SceneObject {
  public:
    Instance(const String &mesh, const String &material);
    ~Instance();

    const String &mesh() const;
    void setMesh(const String &mesh);

    const String &material() const;
    void setMaterial(const String &material);

  private:
    InstancePrivate *d;
  };
}

#endif // INSTANCE_H
