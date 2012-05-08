#include "PointLight.h"

#include "Camera.h"
#include "Material.h"
#include "Mesh.h"
#include "Program.h"
#include "Root.h"
#include "SceneNode.h"

#include <GL/glew.h>

namespace SimpleGL {
  class PointLightPrivate {
  public:
    PointLightPrivate() : attenuationRange(256.0f), attenuationConstant(0.0f), attenuationLinear(0.0f), attenuationQuadratic(1.0f) {
      sphere = Root::instance()->createSphere("", attenuationRange, 32, 32);
    }

    ~PointLightPrivate() {
    }

    Vector3f direction;
    float innerAngle;
    float outerAngle;
    float attenuationRange;
    float attenuationConstant;
    float attenuationLinear;
    float attenuationQuadratic;

    Mesh *sphere;
  };

  PointLight::PointLight() : d(new PointLightPrivate())  {
  }

  PointLight::~PointLight() {
    delete d;
  }

  const LightType PointLight::type() const {
    return LT_POINT;
  }

  const float PointLight::attenuationRange() const {
    return d->attenuationRange;
  }

  const float PointLight::attenuationConstant() const {
    return d->attenuationConstant;
  }

  const float PointLight::attenuationLinear() const {
    return d->attenuationLinear;
  }

  const float PointLight::attenuationQuadratic() const {
    return d->attenuationQuadratic;
  }

  void PointLight::setAttenuation(const float range, const float constant, const float linear, const float quadratic) {
    d->attenuationRange = range;
    d->attenuationConstant = constant;
    d->attenuationLinear = linear;
    d->attenuationQuadratic = quadratic;
    // re-create sphere
    d->sphere = Root::instance()->createSphere("", d->attenuationRange, 32, 32);
  }

  void PointLight::render(Camera *camera) {
    // get program of the material
    Program *program = Root::instance()->retrieveProgram("PointLight");
    if (!program)
      return;
    // adjust face culling
    if (glm::length(camera->parentSceneNode()->worldPosition() - parentSceneNode()->worldPosition()) < d->attenuationRange)
      glCullFace(GL_FRONT);
    else
      glCullFace(GL_BACK);
    // set program parameters
    program->setUniform("lightPos", parentSceneNode()->worldPosition());
    program->setUniform("lightColor", color());
    program->setUniform("lightDiffuseIntensity", diffuseIntensity());
    program->setUniform("lightSpecularIntensity", specularIntensity());
    program->setUniform("lightAttenuationRange", d->attenuationRange);
    program->setUniform("lightAttenuationConstant", d->attenuationConstant);
    program->setUniform("lightAttenuationLinear", d->attenuationLinear);
    program->setUniform("lightAttenuationQuadratic", d->attenuationQuadratic);
    program->setUniform("modelViewProjMatrix", camera->projectionMatrix() * camera->viewMatrix() * parentSceneNode()->worldTransform());
    // render a sphere
    d->sphere->render(camera);
    // reset flags
    glCullFace(GL_BACK);
  }
}
