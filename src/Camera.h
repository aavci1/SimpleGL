#ifndef CAMERA_H
#define CAMERA_H

#include "SceneObject.h"
#include "Types.h"

namespace SimpleGL {
  class CameraPrivate;

  class Camera : public SceneObject {
  public:
    Camera();
    ~Camera();

    const float fov() const;
    void setFov(const float fov);

    const float nearClipDistance() const;
    void setNearClipDistance(const float nearClipDistance);

    const float farClipDistance() const;
    void setFarClipDistance(const float farClipDistance);

    const float aspectRatio() const;
    void setAspectRatio(const float aspectRatio);

    const Matrix4f &viewMatrix() const;
    const Matrix4f &projectionMatrix() const;

  private:
    CameraPrivate *d;
  };
}
#endif // CAMERA_H
