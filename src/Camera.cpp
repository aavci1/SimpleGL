#include "Camera.h"

#include <glm/ext.hpp>

namespace SimpleGL {
  class CameraPrivate {
  public:
    CameraPrivate() : fov(60.0f), nearClipDistance(1.0f), farClipDistance(10000.0f), width(0), height(0) {
    }

    ~CameraPrivate() {
    }

    void recalcProjectionMatrix() {
      projectionMatrix = glm::perspectiveFov(fov, float(width), float(height), nearClipDistance, farClipDistance);
    }

    float fov;
    float nearClipDistance;
    float farClipDistance;
    uint width;
    uint height;

    glm::mat4 projectionMatrix;
  };

  Camera::Camera() : d(new CameraPrivate()) {
  }

  Camera::~Camera() {
    delete d;
  }

  void Camera::setFov(float fov) {
    d->fov = fov;
    // recalc projection matrix
    d->recalcProjectionMatrix();
  }

  void Camera::setNearClipDistance(float nearClipDistance) {
    d->nearClipDistance = nearClipDistance;
    // recalc projection matrix
    d->recalcProjectionMatrix();
  }

  void Camera::setFarClipDistance(float farClipDistance) {
    d->farClipDistance = farClipDistance;
    // recalc projection matrix
    d->recalcProjectionMatrix();
  }

  void Camera::setViewportSize(uint width, uint height) {
    d->width = width;
    d->height = height;
    // recalc projection matrix
    d->recalcProjectionMatrix();
  }

  const glm::mat4 &Camera::projectionMatrix() const {
    return d->projectionMatrix;
  }
}
