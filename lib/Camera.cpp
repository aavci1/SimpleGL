#include "Camera.h"

#include "SceneNode.h"

namespace SimpleGL {
  class CameraPrivate {
  public:
    CameraPrivate() {
    }

    ~CameraPrivate() {
    }

    string name { "" };

    float fov { 60.0f };
    float aspectRatio { 1.33f };
    float nearClipDistance { 1.0f };
    float farClipDistance { 5000.0f };

    Matrix4f viewMatrix;
    Matrix4f projectionMatrix;
  };

  Camera::Camera(const string &name) : d(new CameraPrivate()) {
    d->name = name;
  }

  const string &Camera::name() const {
    return d->name;
  }

  Camera::~Camera() {
    delete d;
  }

  string Camera::type() const {
    return "Camera";
  }

  const float Camera::fov() const {
    return d->fov;
  }

  void Camera::setFov(const float fov) {
    d->fov = fov;
  }

  const float Camera::nearClipDistance() const {
    return d->nearClipDistance;
  }

  void Camera::setNearClipDistance(const float nearClipDistance) {
    d->nearClipDistance = nearClipDistance;
  }

  const float Camera::farClipDistance() const {
    return d->farClipDistance;
  }

  void Camera::setFarClipDistance(const float farClipDistance) {
    d->farClipDistance = farClipDistance;
  }

  const float Camera::aspectRatio() const {
    return d->aspectRatio;
  }

  void Camera::setAspectRatio(const float aspectRatio) {
    d->aspectRatio = aspectRatio;
  }

  const Matrix4f &Camera::viewMatrix() const {
    // set default parameters
    Vector3f eye = Vector3f(0.0f, 0.0f, 0.0f), lookAt = Vector3f(0.0f, 0.0f, -1.0f), up = Vector3f(0.0f, 1.0f, 0.0f);
    // adjust parameters with parent node
    if (parent()) {
      eye = parent()->worldPosition();
      lookAt = parent()->worldPosition() + parent()->worldOrientation() * lookAt;
      up = parent()->worldOrientation() * up;
    }
    // calculate view matrix
    d->viewMatrix = glm::lookAt(eye, lookAt, up);
    // return view matrix
    return d->viewMatrix;
  }

  const Matrix4f &Camera::projectionMatrix() const {
    // calculate projection matrix
    d->projectionMatrix = glm::perspective(d->fov, d->aspectRatio, d->nearClipDistance, d->farClipDistance);
    // return projection matrix
    return d->projectionMatrix;
  }
}
