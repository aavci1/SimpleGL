#include "PointLight.h"

#include "Camera.h"
#include "Material.h"
#include "Model.h"
#include "Program.h"
#include "Root.h"
#include "SceneNode.h"

#include <GL/glew.h>

namespace SimpleGL {
  class PointLightPrivate {
  public:
    PointLightPrivate() {
      sphere = Root::instance()->createSphere("", attenuationRange, 32, 32);
    }

    ~PointLightPrivate() {
    }

    ModelPtr sphere { nullptr };

    float attenuationRange { 256.0f };
    float attenuationConstant { 0.0f };
    float attenuationLinear { 0.0f };
    float attenuationQuadratic { 1.0f };
  };

  PointLight::PointLight() : d(new PointLightPrivate())  {
  }

  PointLight::~PointLight() {
    delete d;
  }

  string PointLight::type() const {
    return "Light/Point";
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

  void PointLight::render(CameraPtr camera) {
    // get program of the material
    ProgramPtr program = Root::instance()->retrieveProgram("Light/Point");
    if (!program)
      return;
    // adjust face culling
    if (glm::length(camera->parent()->worldPosition() - parent()->worldPosition()) < d->attenuationRange)
      glCullFace(GL_FRONT);
    else
      glCullFace(GL_BACK);
    // set program parameters
    program->setUniform("lightPos", parent()->worldPosition());
    program->setUniform("lightColor", color());
    program->setUniform("lightDiffuseIntensity", diffuseIntensity());
    program->setUniform("lightSpecularIntensity", specularIntensity());
    program->setUniform("lightAttenuationRange", d->attenuationRange);
    program->setUniform("lightAttenuationConstant", d->attenuationConstant);
    program->setUniform("lightAttenuationLinear", d->attenuationLinear);
    program->setUniform("lightAttenuationQuadratic", d->attenuationQuadratic);
    program->setUniform("modelViewProjMatrix", camera->projectionMatrix() * camera->viewMatrix() * parent()->worldTransform());
    // render a sphere
    d->sphere->render(camera);
    // reset flags
    glCullFace(GL_BACK);
  }
}
