#include "Camera.h"

#include "Sphere.h"

#include <cmath>
#include <cstdio>

namespace SimpleGL {
  class CameraPrivate {
  public:
    CameraPrivate() : position(0, 0, 0), orientation(1, 0, 0, 0), lookAt(0, 0, 0), up(0, 1, 0), fov(60.0f), nearClipDistance(1.0f), farClipDistance(5000.0f), aspectRatio(1.33) {
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

    void recalcBoundingSphere() {
      // calculate the radius of the frustum sphere
      float clipDistance = farClipDistance - nearClipDistance;
      // use some trig to find the height of the frustum at the far plane
      float height = clipDistance * tan(fov * 180.0f / M_PI * 0.5f);
      // with an aspect ratio of 1, the width will be the same
      float width = height * aspectRatio;
      // halfway point between near/far planes starting at the origin and extending along the z axis
      glm::vec3 P(0.0f, 0.0f, nearClipDistance + clipDistance * 0.5f);
      // the calculate far corner of the frustum
      glm::vec3 Q(width, height, clipDistance);
      // the radius becomes the length of this vector
      boundingSphere.setRadius(glm::length(P - Q));
      // get the look vector of the camera from the view matrix
      glm::vec3 lookVector = glm::vec3(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);
      // calculate the center of the sphere
      boundingSphere.setCenter(position + (lookVector * (clipDistance * 0.5f) + nearClipDistance));
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

    Sphere boundingSphere;
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
    d->recalcBoundingSphere();
  }

  void Camera::moveRelative(const glm::vec3 &translation) {
    d->position += d->orientation * translation;
    d->lookAt += d->orientation * translation;
    // recalc view matrix
    d->recalcViewMatrix();
    d->recalcBoundingSphere();
  }

  void Camera::moveRelative(float x, float y, float z) {
    moveRelative(glm::vec3(x, y, z));
  }

  void Camera::setOrientation(const glm::quat &orientation) {
    d->orientation = glm::normalize(orientation);
    // recalc view matrix
    d->recalcViewMatrix();
    d->recalcBoundingSphere();
  }

  void Camera::setOrientation(float w, float x, float y, float z) {
    d->orientation = glm::normalize(glm::quat(w, x, y, z));
    // recalc view matrix
    d->recalcViewMatrix();
    d->recalcBoundingSphere();
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
    d->recalcBoundingSphere();
  }

  void Camera::lookAt(float x, float y, float z) {
    d->lookAt = glm::vec3(x, y, z);
    // recalc view matrix
    d->recalcViewMatrix();
    d->recalcBoundingSphere();
  }

  void Camera::setFov(float fov) {
    d->fov = fov;
    // recalc projection matrix
    d->recalcProjectionMatrix();
    d->recalcBoundingSphere();
  }

  void Camera::setNearClipDistance(float nearClipDistance) {
    d->nearClipDistance = nearClipDistance;
    // recalc projection matrix
    d->recalcProjectionMatrix();
    d->recalcBoundingSphere();
  }

  void Camera::setFarClipDistance(float farClipDistance) {
    d->farClipDistance = farClipDistance;
    // recalc projection matrix
    d->recalcProjectionMatrix();
    d->recalcBoundingSphere();
  }

  void Camera::setAspectRatio(float aspectRatio) {
    d->aspectRatio = aspectRatio;
    // recalc projection matrix
    d->recalcProjectionMatrix();
    d->recalcBoundingSphere();
  }

  const glm::mat4 &Camera::viewMatrix() const {
    return d->viewMatrix;
  }

  const glm::mat4 &Camera::projectionMatrix() const {
    return d->projectionMatrix;
  }

  const Sphere &Camera::boundingSphere() const {
    return d->boundingSphere;
  }
}
