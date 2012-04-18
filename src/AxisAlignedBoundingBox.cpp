#include "AxisAlignedBoundingBox.h"

namespace SimpleGL {
  class AxisAlignedBoundingBoxPrivate {
  public:
    AxisAlignedBoundingBoxPrivate(const glm::vec3 &minimum, const glm::vec3 &maximum) : minimum(minimum), maximum(maximum) {
    }
    ~AxisAlignedBoundingBoxPrivate() {
    }

    glm::vec3 minimum;
    glm::vec3 maximum;
  };

  AxisAlignedBoundingBox::AxisAlignedBoundingBox(glm::vec3 minimum, glm::vec3 maximum) : d(new AxisAlignedBoundingBoxPrivate(minimum, maximum)) {
  }

  AxisAlignedBoundingBox::~AxisAlignedBoundingBox() {
    delete d;
  }

  void AxisAlignedBoundingBox::setMinimum(const glm::vec3 &minimum) {
    d->minimum = glm::min(d->maximum, minimum);
  }

  const glm::vec3 &AxisAlignedBoundingBox::minimum() const {
    return d->minimum;
  }

  void AxisAlignedBoundingBox::setMaximum(const glm::vec3 &maximum) {
    d->maximum = glm::max(d->minimum, maximum);
  }

  const glm::vec3 &AxisAlignedBoundingBox::maximum() const {
    return d->maximum;
  }

  void AxisAlignedBoundingBox::merge(float x, float y, float z) {
    merge(glm::vec3(x, y, z));
  }

  void AxisAlignedBoundingBox::merge(const glm::vec3 &point) {
    d->minimum = glm::min(d->minimum, point);
    d->maximum = glm::max(d->maximum, point);
  }

  void AxisAlignedBoundingBox::merge(const AxisAlignedBoundingBox &aabb) {
    d->minimum = glm::min(d->minimum, aabb.minimum());
    d->maximum = glm::max(d->maximum, aabb.maximum());
  }
}
