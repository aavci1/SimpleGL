#include "Camera.h"

namespace SimpleGL {
  class CameraPrivate {
  public:
    CameraPrivate() : position(0, 0, 0), orientation(1, 0, 0, 0), lookAt(0, 0, 0), up(0, 1, 0), fov(60.0f), nearClipDistance(1.0f), farClipDistance(10000.0f), aspectRatio(1.33) {
      recalcViewMatrix();
      recalcProjectionMatrix();
    }

    ~CameraPrivate() {
    }

    void recalcViewMatrix() {
      viewMatrix = glm::lookAt(position, orientation * (lookAt - position) + position, up);
    }

    void recalcProjectionMatrix() {
      projectionMatrix = glm::perspective(fov, aspectRatio, nearClipDistance, farClipDistance);
    }


    float fov;
    float nearClipDistance;
    float farClipDistance;
    float aspectRatio;

    glm::vec3 position;
    glm::quat orientation;
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

  void Camera::moveRelative(const glm::vec3 &translation) {
    d->position += d->orientation * translation;
    d->lookAt += d->orientation * translation;
    // recalc view matrix
    d->recalcViewMatrix();
  }

  void Camera::moveRelative(float x, float y, float z) {
    moveRelative(glm::vec3(x, y, z));
  }

  void Camera::setOrientation(const glm::quat &orientation) {
    d->orientation = glm::normalize(orientation);
    // recalc view matrix
    d->recalcViewMatrix();
  }

  void Camera::setOrientation(float w, float x, float y, float z) {
    d->orientation = glm::normalize(glm::quat(w, x, y, z));
    // recalc view matrix
    d->recalcViewMatrix();
  }

  const glm::quat &Camera::orientation() const {
    return d->orientation;
  }

  void Camera::rotate(float angle, const glm::vec3 &axis, TransformSpace transformSpace) {
    if (transformSpace == TS_WORLD)
      setOrientation(glm::angleAxis(angle, axis) * d->orientation);
    else if (transformSpace == TS_LOCAL)
      setOrientation(d->orientation * glm::angleAxis(angle, axis));
  }

  void Camera::pitch(float angle, TransformSpace transformSpace) {
    rotate(angle, glm::vec3(1.0f, 0.0f, 0.0f), transformSpace);
  }

  void Camera::yaw(float angle, TransformSpace transformSpace) {
    rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f), transformSpace);
  }

  void Camera::roll(float angle, TransformSpace transformSpace) {
    rotate(angle, glm::vec3(0.0f, 0.0f, 1.0f), transformSpace);
  }

  const glm::vec3 &Camera::position() const {
    return d->position;
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
