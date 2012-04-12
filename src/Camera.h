#ifndef CAMERA_H
#define CAMERA_H

#include <sys/types.h>

#include <glm/glm.hpp>

namespace SimpleGL {
  class CameraPrivate;

  class Camera {
  public:
    Camera();
    ~Camera();

    void setFov(float fov);
    void setNearClipDistance(float nearClipDistance);
    void setFarClipDistance(float farClipDistance);
    void setViewportSize(uint width, uint height);

    const glm::mat4 &projectionMatrix() const;

  private:
    CameraPrivate *d;
  };
}

#endif // CAMERA_H
