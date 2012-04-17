#include "Renderer.h"

#include "Attribute.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "FragmentShader.h"
#include "GBuffer.h"
#include "Light.h"
#include "Material.h"
#include "MaterialManager.h"
#include "Node.h"
#include "PointLight.h"
#include "Program.h"
#include "Quad.h"
#include "Sphere.h"
#include "Util.h"
#include "VertexShader.h"

#include <GL/glew.h>

#include <FreeImage.h>

#include <algorithm>

namespace SimpleGL {
  class RendererPrivate {
  public:
    RendererPrivate(uint width, uint height) : width(width), height(height), gbuffer(new GBuffer(width, height)), pointLightProgram(0), directionalLightProgram(0), quad(new Quad()) {
    }

    ~RendererPrivate() {
      delete gbuffer;
      delete pointLightProgram;
      delete directionalLightProgram;
      delete quad;
    }

    void renderHelper(Node *node, glm::mat4 modelMatrix, glm::mat4 viewProjMatrix) {
      modelMatrix *= node->transformationMatrix();
      // visit child nodes
      for (int i = 0; i < node->nodes().size(); ++i)
        renderHelper(node->nodes().at(i), modelMatrix, viewProjMatrix);
      // add lights to the list
      for (int i = 0; i < node->lights().size(); ++i)
        lights.push_back(node->lights().at(i));
      // render meshes
      for (int i = 0; i < node->meshes().size(); ++i) {
        Material *material = MaterialManager::instance()->getMaterialByName(node->meshes().at(i)->materialName());
        if (!material)
          continue;
        // select program
        material->select();
        // update uniforms
        material->program()->setUniform("sglModelMatrix", modelMatrix);
        material->program()->setUniform("sglModelViewProjMatrix", viewProjMatrix * modelMatrix);
        // render the mesh
        node->meshes().at(i)->render();
        // deselect shader
        material->deselect();
      }
    }

    int width;
    int height;
    GBuffer *gbuffer;
    std::vector<Light *> lights;
    Program *pointLightProgram;
    Program *directionalLightProgram;
    Quad *quad;
  };

  Renderer::Renderer(uint width, uint height) : d(new RendererPrivate(width, height)) {
    // load point light program
    d->pointLightProgram = new Program();
    d->pointLightProgram->addShader(new VertexShader(Util::readAll("media/deferred_light_point_vp.glsl")));
    d->pointLightProgram->addShader(new FragmentShader(Util::readAll("media/deferred_light_point_fp.glsl")));
    if (!d->pointLightProgram->compileAndLink())
      printf("error: can not compile shader:\n%s", d->pointLightProgram->message().c_str());
    // load directional light program
    d->directionalLightProgram = new Program();
    d->directionalLightProgram->addShader(new VertexShader(Util::readAll("media/deferred_light_directional_vp.glsl")));
    d->directionalLightProgram->addShader(new FragmentShader(Util::readAll("media/deferred_light_directional_fp.glsl")));
    if (!d->directionalLightProgram->compileAndLink())
      printf("error: can not compile shader:\n%s", d->directionalLightProgram->message().c_str());
  }

  Renderer::~Renderer() {
    delete d;
  }

  void Renderer::setSize(uint width, uint height) {
    d->width = width;
    d->height = height;
    delete d->gbuffer;
    d->gbuffer = new GBuffer(width, height);
  }

  void Renderer::renderOneFrame(Camera *camera, Node *root) {
    // GEOMETRY PASS
    // bind gbuffer for writing
    d->gbuffer->setWritable(true);
    // set general parameters
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    // set parameters for the geometry pass
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    // clear color and depth buffers
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // clear lights
    d->lights.clear();
    // render scene
    d->renderHelper(root, glm::mat4(), camera->projectionMatrix() * camera->viewMatrix());
    // unbind gbuffer
    d->gbuffer->setWritable(false);
    // bind textures
    d->gbuffer->bindTextures();
    // set parameters for lighting pass
    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);
    // clear color buffer
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
#if 0
    // blit gbuffer to the screen
    d->gbuffer->blit();
#else
    // point lights pass
    d->pointLightProgram->select();
    d->pointLightProgram->setUniform("screenSize", glm::vec2(d->width, d->height));
    d->pointLightProgram->setUniform("cameraPos", camera->position());
    d->pointLightProgram->setUniform("colorBuffer", d->gbuffer->colorBuffer());
    d->pointLightProgram->setUniform("normalBuffer", d->gbuffer->normalBuffer());
    d->pointLightProgram->setUniform("positionBuffer", d->gbuffer->positionBuffer());
    for (int i = 0;  i < d->lights.size(); ++i) {
      if (d->lights.at(i)->type() == LT_POINT) {
        PointLight *light = static_cast<PointLight *>(d->lights.at(i));
        if (glm::length(camera->position() - light->position()) < light->attenuationRange())
          glCullFace(GL_FRONT);
        else
          glCullFace(GL_BACK);
        // set light parameters
        d->pointLightProgram->setUniform("sglModelViewProjMatrix", camera->projectionMatrix() * camera->viewMatrix() * light->transformationMatrix());
        d->pointLightProgram->setUniform("lightPos", light->position());
        d->pointLightProgram->setUniform("lightColor", light->color());
        d->pointLightProgram->setUniform("lightAttenuationRange", light->attenuationRange());
        d->pointLightProgram->setUniform("lightAttenuationConstant", light->attenuationConstant());
        d->pointLightProgram->setUniform("lightAttenuationLinear", light->attenuationLinear());
        d->pointLightProgram->setUniform("lightAttenuationQuadratic", light->attenuationQuadratic());
        d->pointLightProgram->setUniform("lightDiffuseIntensity", light->diffuseIntensity());
        d->pointLightProgram->setUniform("lightSpecularIntensity", light->specularIntensity());
        // draw a sphere
        Sphere *sphere = new Sphere(light->attenuationRange());
        sphere->render();
        delete sphere;
      }
    }
    // deselect shader
    d->pointLightProgram->deselect();
    // reset parameters
    glCullFace(GL_BACK);
    // directional lights pass
    d->directionalLightProgram->select();
    d->directionalLightProgram->setUniform("screenSize", glm::vec2(d->width, d->height));
    d->directionalLightProgram->setUniform("cameraPos", camera->position());
    d->directionalLightProgram->setUniform("colorBuffer", d->gbuffer->colorBuffer());
    d->directionalLightProgram->setUniform("normalBuffer", d->gbuffer->normalBuffer());
    d->directionalLightProgram->setUniform("positionBuffer", d->gbuffer->positionBuffer());
    for (int i = 0;  i < d->lights.size(); ++i) {
      if (d->lights.at(i)->type() == LT_DIRECTIONAL) {
        DirectionalLight *light = static_cast<DirectionalLight *>(d->lights.at(i));
        // set light parameters
        d->directionalLightProgram->setUniform("lightDir", light->direction());
        d->directionalLightProgram->setUniform("lightColor", light->color());
        d->directionalLightProgram->setUniform("lightDiffuseIntensity", light->diffuseIntensity());
        d->directionalLightProgram->setUniform("lightSpecularIntensity", light->specularIntensity());
        // render full screen quad
        d->quad->render();
      }
    }
    // deselect shader
    d->directionalLightProgram->deselect();
    // unbind textures
    d->gbuffer->unbindTextures();
#endif
  }

  bool Renderer::saveScreenshot(const std::string &path) {
    // get file extension
    std::string extension = path.substr(path.find_last_of(".") + 1);
    // convert to upper case
    std::transform(extension.begin(), extension.end(), extension.begin(), ::toupper);
    // check if file extension supported
    if (extension != "JPG" && extension != "JPEG" && extension != "PNG" && extension != "BMP")
      return false;
    // create a buffer to hold pixels
    BYTE* buffer = new BYTE[3 * d->width * d->height];
    // read pixels from the buffer
    glReadPixels(0, 0, d->width, d->height, GL_BGR, GL_UNSIGNED_BYTE, buffer);
    // convert pixels to image
    FIBITMAP* image = FreeImage_ConvertFromRawBits(buffer, d->width, d->height, 3 * d->width, 24, 0x0000FF, 0xFF0000, 0x00FF00, false);
    // save image depending on format
    if (extension == "JPG" || extension == "JPEG")
      FreeImage_Save(FIF_JPEG, image, path.c_str(), 0);
    else if (extension == "PNG")
      FreeImage_Save(FIF_PNG, image, path.c_str(), 0);
    else if (extension == "BMP")
      FreeImage_Save(FIF_BMP, image, path.c_str(), 0);
    // delete image
    delete image;
    // delete buffer
    delete[] buffer;
    // return success
    return true;
  }
}
