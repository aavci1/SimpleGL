#include "Camera.h"

#include "SceneNode.h"

namespace SimpleGL {
  class CameraPrivate {
  public:
    CameraPrivate() {
    }

    ~CameraPrivate() {
    }

    Vector3f direction { 0.0f, 0.0f, -1.0f };

    float fov { 60.0f };
    float aspectRatio { 1.33f };
    float nearClipDistance { 1.0f };
    float farClipDistance { 5000.0f };

    bool recalcViewMatrix { true };
    Matrix4f viewMatrix;

    bool recalcProjectionMatrix { true };
    Matrix4f projectionMatrix;
  };

  Camera::Camera() : d(new CameraPrivate()) {
  }

  Camera::~Camera() {
    delete d;
  }

  const float Camera::fov() const {
    return d->fov;
  }

  void Camera::setFov(const float fov) {
    d->fov = fov;
    // recalc projection matrix
    d->recalcProjectionMatrix = true;
  }

  const float Camera::nearClipDistance() const {
    return d->nearClipDistance;
  }

  void Camera::setNearClipDistance(const float nearClipDistance) {
    d->nearClipDistance = nearClipDistance;
    // recalc projection matrix
    d->recalcProjectionMatrix = true;
  }

  const float Camera::farClipDistance() const {
    return d->farClipDistance;
  }

  void Camera::setFarClipDistance(const float farClipDistance) {
    d->farClipDistance = farClipDistance;
    // recalc projection matrix
    d->recalcProjectionMatrix = true;
  }

  const float Camera::aspectRatio() const {
    return d->aspectRatio;
  }

  void Camera::setAspectRatio(const float aspectRatio) {
    d->aspectRatio = aspectRatio;
    // recalc projection matrix
    d->recalcProjectionMatrix = true;
  }

  const Matrix4f &Camera::viewMatrix() const {
    if (d->recalcViewMatrix) {
      // calculate eye, lookAt and up vectors
      Vector3f eye = (parentSceneNode() != 0) ? parentSceneNode()->worldPosition() : Vector3f(0.0f, 0.0f, 0.0f);
      Vector3f lookAt = (parentSceneNode() != 0) ? parentSceneNode()->worldPosition() + parentSceneNode()->worldOrientation() * d->direction : Vector3f(0.0f, 0.0f, -1.0f);
      Vector3f up = (parentSceneNode() != 0) ? parentSceneNode()->worldOrientation() * Vector3f(0.0f, 1.0f, 0.0f) : Vector3f(0.0f, 1.0f, 0.0f);
      // calculate view matrix
      d->viewMatrix = glm::lookAt(eye, lookAt, up);
      // FIXME: dont reset flag, because parent transformation may change out of our control
      // d->recalcViewMatrix = false;
    }
    // return view matrix
    return d->viewMatrix;
  }

  const Matrix4f &Camera::projectionMatrix() const {
    if (d->recalcProjectionMatrix) {
      // calculate projection matrix
      d->projectionMatrix = glm::perspective(d->fov, d->aspectRatio, d->nearClipDistance, d->farClipDistance);
      // reset flag
      d->recalcProjectionMatrix = false;
    }
    // return projection matrix
    return d->projectionMatrix;
  }
}
