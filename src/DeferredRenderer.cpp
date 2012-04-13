#include "DeferredRenderer.h"

#include "Attribute.h"
#include "Camera.h"
#include "CubeMesh.h"
#include "FragmentShader.h"
#include "GBuffer.h"
#include "Light.h"
#include "Node.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "VertexShader.h"

#include <GL/glew.h>

#include <fstream>
#include <iostream>
#include <sstream>

namespace SimpleGL {
  class DeferredRendererPrivate {
  public:
    DeferredRendererPrivate(uint width, uint height) : width(width), height(height), gbuffer(new GBuffer(width, height)), directionLightShader(0) {
    }

    ~DeferredRendererPrivate() {
      delete gbuffer;
      delete directionLightShader;
      // clean up
      delete texture;
      delete shaderProgram;
    }

    std::string readAll(const char *fileName) {
      std::ifstream in(fileName);
      std::stringstream buffer;
      buffer << in.rdbuf();

      return std::string(buffer.str());
    }

    void renderHelper(Node *node, glm::mat4 modelViewProj) {
      modelViewProj = modelViewProj * node->transformationMatrix();
      // visit child nodes
      for (int i = 0; i < node->nodes().size(); ++i)
        renderHelper(node->nodes().at(i), modelViewProj);
      // add lights to the list
      for (int i = 0; i < node->lights().size(); ++i)
        lights.push_back(node->lights().at(i));
      // render meshes
      for (int i = 0; i < node->meshes().size(); ++i) {
        // select texture
        texture->select(0);
        // update uniforms
        shaderProgram->setUniform("sglModelViewProjMatrix", modelViewProj);
        shaderProgram->setUniform("sglSampler", 0);
        // select shader
        shaderProgram->select();
        // render the mesh
        node->meshes().at(i)->render();
        // deselect shader
        shaderProgram->deselect();
        // deselect texture
        texture->deselect();
      }
    }

    int width;
    int height;
    GBuffer *gbuffer;
    std::vector<Light *> lights;
    ShaderProgram *directionLightShader;
    Texture *texture;
    ShaderProgram *shaderProgram;
  };

  DeferredRenderer::DeferredRenderer(uint width, uint height) : Renderer(), d(new DeferredRendererPrivate(width, height)) {
    // load directional light shader
    d->directionLightShader = new ShaderProgram();
    d->directionLightShader->addShader(new VertexShader(d->readAll("media/deferred_directional_light_vp.glsl")));
    d->directionLightShader->addShader(new FragmentShader(d->readAll("media/deferred_directional_light_fp.glsl")));
    if (!d->directionLightShader->compileAndLink())
      printf("error: can not compile shader:\n%s", d->directionLightShader->message().c_str());
    // load texture
    d->texture = new Texture("media/laminate.jpg");
    if (!d->texture->load())
      printf("error: can not load texture %s.", d->texture->path().c_str());
    // load shader
    d->shaderProgram = new ShaderProgram();
    d->shaderProgram->addShader(new VertexShader(d->readAll("media/deferred_vp.glsl")));
    d->shaderProgram->addShader(new FragmentShader(d->readAll("media/deferred_fp.glsl")));
    if (!d->shaderProgram->compileAndLink())
      printf("error: can not compile shader:\n%s", d->shaderProgram->message().c_str());
  }

  DeferredRenderer::~DeferredRenderer() {
    delete d;
  }

  void DeferredRenderer::renderOneFrame(Camera *camera, Node *root) {
    // GEOMETRY PASS
    // bind gbuffer for writing
    d->gbuffer->setWritable(true);
    // enable depth mask
    glDepthMask(GL_TRUE);
    // enable depth test
    glEnable(GL_DEPTH_TEST);
    // disable blending
    glDisable(GL_BLEND);
    // clear color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // clear lights
    d->lights.clear();
    // render scene
    d->renderHelper(root, camera->projectionMatrix() * camera->viewMatrix());
    // unbind gbuffer
    d->gbuffer->setWritable(false);
    // LIGHTING PASS
    // disable depth mask
    glDepthMask(GL_FALSE);
    // disable depth test
    glDisable(GL_DEPTH_TEST);
    // bind gbuffer for reading
    d->gbuffer->setReadable();
    // clear color buffer
    glClear(GL_COLOR_BUFFER_BIT);
    // set blending
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);
    // TODO: points lights pass
    // directional lights pass
    d->directionLightShader->setUniform("screenSize", glm::vec2(d->width, d->height));
    d->directionLightShader->setUniform("cameraPos", glm::vec3(0, 170, 1000)); // TODO: get from camera
    d->directionLightShader->setUniform("colorSampler", d->gbuffer->colorSampler());
    d->directionLightShader->setUniform("normalSampler", d->gbuffer->normalSampler());
    d->directionLightShader->setUniform("positionSampler", d->gbuffer->positionSampler());
    d->directionLightShader->setUniform("depthSampler", 3);
    for (int i = 0;  i < d->lights.size(); ++i) {
      if (d->lights.at(i)->type() == LT_DIRECTIONAL) {
        d->directionLightShader->setUniform("direction", d->lights.at(i)->direction());
        d->directionLightShader->setUniform("ambientColor", d->lights.at(i)->ambientColor());
        d->directionLightShader->setUniform("diffuseColor", d->lights.at(i)->diffuseColor());
        d->directionLightShader->setUniform("specularColor", d->lights.at(i)->specularColor());
        d->directionLightShader->select();
        CubeMesh *mesh = new CubeMesh();
        mesh->render();
        d->directionLightShader->deselect();
      }
    }
//    d->gbuffer->blit();
  }
}
