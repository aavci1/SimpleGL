#include "Instance.h"

namespace SimpleGL {
  class InstancePrivate {
  public:
    InstancePrivate() {
    }

    ~InstancePrivate() {
    }

    string model { "" };
    string material { "" };
  };

  Instance::Instance(const string &model, const string &material) : d(new InstancePrivate()) {
    d->model = model;
    d->material = material;
  }

  Instance::~Instance() {
    delete d;
  }

  string Instance::type() const {
    return "Instance";
  }

  const string &Instance::model() const {
    return d->model;
  }

  void Instance::setModel(const string &model) {
    d->model = model;
  }

  const string &Instance::material() const {
    return d->material;
  }

  void Instance::setMaterial(const string &material) {
    d->material = material;
  }
}
