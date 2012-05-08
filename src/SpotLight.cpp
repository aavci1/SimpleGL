#include "SpotLight.h"

#include "Camera.h"
#include "Material.h"
#include "Mesh.h"
#include "Program.h"
#include "Root.h"
#include "SceneNode.h"

#include <GL/glew.h>

namespace SimpleGL {
  class SpotLightPrivate {
  public:
    SpotLightPrivate() : direction(0.0f, 0.0f, -1.0f), innerAngle(30), outerAngle(30), attenuationRange(256.0f), attenuationConstant(0.0f), attenuationLinear(0.0f), attenuationQuadratic(1.0f) {
      cone = Root::instance()->createCone("", tanf(innerAngle + outerAngle) * attenuationRange, attenuationRange);
    }

    ~SpotLightPrivate() {
    }

    Vector3f direction;
    float innerAngle;
    float outerAngle;
    float attenuationRange;
    float attenuationConstant;
    float attenuationLinear;
    float attenuationQuadratic;

    Mesh *cone;
  };

  SpotLight::SpotLight() : d(new SpotLightPrivate())  {
  }

  SpotLight::~SpotLight() {
    delete d;
  }

  const LightType SpotLight::type() const {
    return LT_SPOT;
  }

  const Vector3f &SpotLight::direction() const {
    return d->direction;
  }

  void SpotLight::setDirection(const Vector3f &direction) {
    d->direction = glm::normalize(direction);
  }

  void SpotLight::setDirection(const float x, const float y, const float z) {
    setDirection(Vector3f(x, y, z));
  }

  const float SpotLight::innerAngle() const {
    return d->innerAngle;
  }

  void SpotLight::setInnerAngle(const float angle) {
    d->innerAngle = angle;
  }

  const float SpotLight::outerAngle() const {
    return d->outerAngle;
  }

  void SpotLight::setOuterAngle(const float angle) {
    d->outerAngle = angle;
    // re-create cone
    d->cone = Root::instance()->createCone("", tanf((d->innerAngle + d->outerAngle) * M_PI / 180) * d->attenuationRange, d->attenuationRange);
  }

  const float SpotLight::attenuationRange() const {
    return d->attenuationRange;
  }

  const float SpotLight::attenuationConstant() const {
    return d->attenuationConstant;
  }

  const float SpotLight::attenuationLinear() const {
    return d->attenuationLinear;
  }

  const float SpotLight::attenuationQuadratic() const {
    return d->attenuationQuadratic;
  }

  void SpotLight::setAttenuation(const float range, const float constant, const float linear, const float quadratic) {
    d->attenuationRange = range;
    d->attenuationConstant = constant;
    d->attenuationLinear = linear;
    d->attenuationQuadratic = quadratic;
    // re-create cone
    d->cone = Root::instance()->createCone("", tanf((d->innerAngle + d->outerAngle) * M_PI / 180) * d->attenuationRange, d->attenuationRange);
  }

  void SpotLight::render(Camera *camera) {
    Program *program = Root::instance()->retrieveProgram("SpotLight");
    if (!program)
      return;
    // adjust face culling
    Vector3f direction = parentSceneNode()->worldOrientation() * d->direction;
    float cos = cosf((d->innerAngle + d->outerAngle) * M_PI / 180);
    Vector3f cameraDir = glm::normalize(camera->parentSceneNode()->worldPosition() - parentSceneNode()->worldPosition());
    if (glm::dot(cameraDir, direction) >= cos)
      glCullFace(GL_FRONT);
    else
      glCullFace(GL_BACK);
    // set program parameters
    program->setUniform("lightColor", color());
    program->setUniform("lightDiffuseIntensity", diffuseIntensity());
    program->setUniform("lightSpecularIntensity", specularIntensity());
    program->setUniform("lightPos", parentSceneNode()->worldPosition());
    program->setUniform("lightDirection", parentSceneNode()->worldOrientation() * direction);
    program->setUniform("lightInnerAngle", float(d->innerAngle * M_PI / 180.0f));
    program->setUniform("lightOuterAngle", float(d->outerAngle * M_PI / 180.0f));
    program->setUniform("lightAttenuationRange", d->attenuationRange);
    program->setUniform("lightAttenuationConstant", d->attenuationConstant);
    program->setUniform("lightAttenuationLinear", d->attenuationLinear);
    program->setUniform("lightAttenuationQuadratic", d->attenuationQuadratic);
    program->setUniform("modelViewProjMatrix", camera->projectionMatrix() * camera->viewMatrix() * parentSceneNode()->transform());
    // render a cone
    d->cone->render(camera);
    // reset flags
    glCullFace(GL_BACK);
  }
}
