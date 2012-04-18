#ifndef AXISALIGNEDBOUNDINGBOX_H
#define AXISALIGNEDBOUNDINGBOX_H

#include <glm/glm.hpp>

namespace SimpleGL {
  class AxisAlignedBoundingBoxPrivate;

  class AxisAlignedBoundingBox {
  public:
    AxisAlignedBoundingBox(glm::vec3 minimum = glm::vec3(FLT_MAX), glm::vec3 maximum = glm::vec3(-FLT_MAX));
    ~AxisAlignedBoundingBox();

    void setMinimum(const glm::vec3 &minimum);
    const glm::vec3 &minimum() const;

    void setMaximum(const glm::vec3 &maximum);
    const glm::vec3 &maximum() const;

    void merge(float x, float y, float z);
    void merge(const glm::vec3 &point);
    void merge(const AxisAlignedBoundingBox &aabb);

  private:
    AxisAlignedBoundingBoxPrivate *d;
  };
}

#endif // AXISALIGNEDBOUNDINGBOX_H
