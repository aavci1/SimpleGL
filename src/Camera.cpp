#include "Camera.h"

#include <glm/ext.hpp>

namespace SimpleGL {
  class CameraPrivate {
  public:
    CameraPrivate() : position(0, 0, 0), lookAt(0, 0, -1), up(0, 1, 0), fov(60.0f), nearClipDistance(1.0f), farClipDistance(10000.0f), aspectRatio(1.33) {
      recalcViewMatrix();
      recalcProjectionMatrix();
    }

    ~CameraPrivate() {
    }

    void recalcViewMatrix() {
      viewMatrix = glm::lookAt(position, lookAt, glm::normalize(up));
    }

    void recalcProjectionMatrix() {
      projectionMatrix = glm::perspective(fov, aspectRatio, nearClipDistance, farClipDistance);
    }


    float fov;
    float nearClipDistance;
    float farClipDistance;
    float aspectRatio;

    glm::vec3 position;
    glm::vec3 lookAt;
    glm::vec3 up;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
  };

  Camera::Camera() : d(new CameraPrivate()) {
  }

  Camera::~Camera() {
    delete d;
  }

  void Camera::setPosition(const glm::vec3 &position) {
    d->position = position;
    // recalc view matrix
    d->recalcViewMatrix();
  }

  void Camera::setPosition(float x, float y, float z) {
    d->position = glm::vec3(x, y, z);
    // recalc view matrix
    d->recalcViewMatrix();
  }

  void Camera::lookAt(const glm::vec3 &lookAt) {
    d->lookAt = lookAt;
    // recalc view matrix
    d->recalcViewMatrix();
  }

  void Camera::lookAt(float x, float y, float z) {
    d->lookAt = glm::vec3(x, y, z);
    // recalc view matrix
    d->recalcViewMatrix();
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

  void Camera::setAspectRatio(float aspectRatio) {
    d->aspectRatio = aspectRatio;
    // recalc projection matrix
    d->recalcProjectionMatrix();
  }

  const glm::mat4 &Camera::viewMatrix() const {
    return d->viewMatrix;
  }

  const glm::mat4 &Camera::projectionMatrix() const {
    return d->projectionMatrix;
  }
}
