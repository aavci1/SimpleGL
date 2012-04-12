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

    void setPosition(const glm::vec3 &position);
    void setPosition(float x, float y, float z);

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
