#ifndef INSTANCE_H
#define INSTANCE_H

#include "SceneObject.h"
#include "Types.h"

namespace SimpleGL {
  class InstancePrivate;

  class Instance : public SceneObject {
  public:
    Instance(const string &mesh, const string &material);
    ~Instance();

    string type() const;

    const string &mesh() const;
    void setMesh(const string &mesh);

    const string &material() const;
    void setMaterial(const string &material);

  private:
    InstancePrivate *d;
  };
}

#endif // INSTANCE_H
