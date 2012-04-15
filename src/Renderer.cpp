#include "Renderer.h"

#include "Attribute.h"
#include "Camera.h"
#include "FragmentShader.h"
#include "GBuffer.h"
#include "Light.h"
#include "Node.h"
#include "Program.h"
#include "Quad.h"
#include "Sphere.h"
#include "Texture.h"
#include "VertexShader.h"

#include <GL/glew.h>

#include <fstream>
#include <iostream>
#include <sstream>

namespace SimpleGL {
  class RendererPrivate {
  public:
    RendererPrivate(uint width, uint height) : width(width), height(height), gbuffer(new GBuffer(width, height)), geometryProgram(0), pointLightProgram(0), directionalLightProgram(0), quad(new Quad()) {
    }

    ~RendererPrivate() {
      delete gbuffer;
      delete texture;
      delete geometryProgram;
      delete pointLightProgram;
      delete directionalLightProgram;
      delete quad;
    }

    std::string readAll(const char *fileName) {
      std::ifstream in(fileName);
      std::stringstream buffer;
      buffer << in.rdbuf();

      return std::string(buffer.str());
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
        // select texture
        texture->select(0);
        // select shader
        geometryProgram->select();
        // update uniforms
        geometryProgram->setUniform("sglModelMatrix", modelMatrix);
        geometryProgram->setUniform("sglModelViewProjMatrix", viewProjMatrix * modelMatrix);
        geometryProgram->setUniform("sglSampler", 0);
        geometryProgram->setUniform("sglSpecularIntensity", 1.0f);
        geometryProgram->setUniform("sglSpecularPower", 8.0f);
        // render the mesh
        node->meshes().at(i)->render();
        // deselect shader
        geometryProgram->deselect();
        // deselect texture
        texture->deselect();
      }
    }

    int width;
    int height;
    GBuffer *gbuffer;
    std::vector<Light *> lights;
    Texture *texture;
    Program *geometryProgram;
    Program *pointLightProgram;
    Program *directionalLightProgram;
    Quad *quad;
  };

  Renderer::Renderer(uint width, uint height) : d(new RendererPrivate(width, height)) {
    // load texture
    d->texture = new Texture("media/laminate.jpg");
    if (!d->texture->load())
      printf("error: can not load texture %s.", d->texture->path().c_str());
    // load geometry program
    d->geometryProgram = new Program();
    d->geometryProgram->addShader(new VertexShader(d->readAll("media/deferred_geometry_vp.glsl")));
    d->geometryProgram->addShader(new FragmentShader(d->readAll("media/deferred_geometry_fp.glsl")));
    if (!d->geometryProgram->compileAndLink())
      printf("error: can not compile shader:\n%s", d->geometryProgram->message().c_str());
    // load point light program
    d->pointLightProgram = new Program();
    d->pointLightProgram->addShader(new VertexShader(d->readAll("media/deferred_light_point_vp.glsl")));
    d->pointLightProgram->addShader(new FragmentShader(d->readAll("media/deferred_light_point_fp.glsl")));
    if (!d->pointLightProgram->compileAndLink())
      printf("error: can not compile shader:\n%s", d->pointLightProgram->message().c_str());
    // load directional light program
    d->directionalLightProgram = new Program();
    d->directionalLightProgram->addShader(new VertexShader(d->readAll("media/deferred_light_directional_vp.glsl")));
    d->directionalLightProgram->addShader(new FragmentShader(d->readAll("media/deferred_light_directional_fp.glsl")));
    if (!d->directionalLightProgram->compileAndLink())
      printf("error: can not compile shader:\n%s", d->directionalLightProgram->message().c_str());
  }

  Renderer::~Renderer() {
    delete d;
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
    d->pointLightProgram->setUniform("colorSampler", d->gbuffer->colorSampler());
    d->pointLightProgram->setUniform("normalSampler", d->gbuffer->normalSampler());
    d->pointLightProgram->setUniform("positionSampler", d->gbuffer->positionSampler());
    for (int i = 0;  i < d->lights.size(); ++i) {
      if (d->lights.at(i)->type() == LT_POINT) {
        if (glm::length(camera->position() - d->lights.at(i)->position()) < d->lights.at(i)->radius())
          glCullFace(GL_FRONT);
        else
          glCullFace(GL_BACK);
        // set light parameters
        d->pointLightProgram->setUniform("sglModelViewProjMatrix", camera->projectionMatrix() * camera->viewMatrix() * d->lights.at(i)->transformationMatrix());
        d->pointLightProgram->setUniform("lightPos", d->lights.at(i)->position());
        d->pointLightProgram->setUniform("lightColor", d->lights.at(i)->color());
        d->pointLightProgram->setUniform("lightRadius", d->lights.at(i)->radius());
        d->pointLightProgram->setUniform("lightDiffuseIntensity", d->lights.at(i)->diffuseIntensity());
        d->pointLightProgram->setUniform("lightSpecularIntensity", d->lights.at(i)->specularIntensity());
        // draw a sphere
        Sphere *sphere = new Sphere(d->lights.at(i)->radius());
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
    d->directionalLightProgram->setUniform("colorSampler", d->gbuffer->colorSampler());
    d->directionalLightProgram->setUniform("normalSampler", d->gbuffer->normalSampler());
    d->directionalLightProgram->setUniform("positionSampler", d->gbuffer->positionSampler());
    for (int i = 0;  i < d->lights.size(); ++i) {
      if (d->lights.at(i)->type() == LT_DIRECTIONAL) {
        // set light parameters
        d->directionalLightProgram->setUniform("lightDir", d->lights.at(i)->direction());
        d->directionalLightProgram->setUniform("lightColor", d->lights.at(i)->color());
        d->directionalLightProgram->setUniform("lightDiffuseIntensity", d->lights.at(i)->diffuseIntensity());
        d->directionalLightProgram->setUniform("lightSpecularIntensity", d->lights.at(i)->specularIntensity());
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
}
