#ifndef CAMERA_H
#define CAMERA_H

#include "TransformSpace.h"

#include <sys/types.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace SimpleGL {
  class CameraPrivate;

  class Camera {
  public:
    Camera();
    ~Camera();

    void setPosition(const glm::vec3 &position);
    void setPosition(float x, float y, float z);
    const glm::vec3 &position() const;

    void setOrientation(const glm::quat &orientation);
    void setOrientation(float w, float x, float y, float z);
    const glm::quat &orientation() const;

    void rotate(float angle, const glm::vec3 &axis, TransformSpace transformSpace = TS_LOCAL);

    void pitch(float angle, TransformSpace transformSpace = TS_LOCAL);
    void yaw(float angle, TransformSpace transformSpace = TS_WORLD);
    void roll(float angle, TransformSpace transformSpace = TS_LOCAL);

    void lookAt(const glm::vec3 &lookAt);
    void lookAt(float x, float y, float z);

    void setFov(float fov);
    void setNearClipDistance(float nearClipDistance);
    void setFarClipDistance(float farClipDistance);
    void setAspectRatio(float aspectRatio);

    const glm::mat4 &viewMatrix() const;
    const glm::mat4 &projectionMatrix() const;

  private:
    CameraPrivate *d;
  };
}

#endif // CAMERA_H
