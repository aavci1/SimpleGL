#ifndef INSTANCE_H
#define INSTANCE_H

#include "SceneObject.h"
#include "Types.h"

namespace SimpleGL {
  class InstancePrivate;

  class Instance : public SceneObject {
  public:
    Instance(const string &model, const string &material);
    ~Instance();

    string type() const;

    const string &model() const;
    void setModel(const string &model);

    const string &material() const;
    void setMaterial(const string &material);

  private:
    InstancePrivate *d;
  };
}

#endif // INSTANCE_H
