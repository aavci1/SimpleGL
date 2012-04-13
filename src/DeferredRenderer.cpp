#include "DeferredRenderer.h"

#include "Attribute.h"
#include "Camera.h"
#include "FragmentShader.h"
#include "GBuffer.h"
#include "Light.h"
#include "Node.h"
#include "QuadMesh.h"
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
    DeferredRendererPrivate(uint width, uint height) : width(width), height(height), gbuffer(new GBuffer(width, height)), directionalLightShader(0), quad(new QuadMesh()) {
    }

    ~DeferredRendererPrivate() {
      delete gbuffer;
      delete directionalLightShader;
      delete quad;
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
        // select shader
        shaderProgram->select();
        // update uniforms
        shaderProgram->setUniform("sglModelViewProjMatrix", modelViewProj);
        shaderProgram->setUniform("sglSampler", 0);
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
    ShaderProgram *directionalLightShader;
    QuadMesh *quad;
    Texture *texture;
    ShaderProgram *shaderProgram;
  };

  DeferredRenderer::DeferredRenderer(uint width, uint height) : Renderer(), d(new DeferredRendererPrivate(width, height)) {
    // load directional light shader
    d->directionalLightShader = new ShaderProgram();
    d->directionalLightShader->addShader(new VertexShader(d->readAll("media/deferred_directional_light_vp.glsl")));
    d->directionalLightShader->addShader(new FragmentShader(d->readAll("media/deferred_directional_light_fp.glsl")));
    if (!d->directionalLightShader->compileAndLink())
      printf("error: can not compile shader:\n%s", d->directionalLightShader->message().c_str());
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
    d->directionalLightShader->select();
    d->directionalLightShader->setUniform("screenSize", glm::vec2(d->width, d->height));
    d->directionalLightShader->setUniform("cameraPos", glm::vec3(0, 170, 1000)); // TODO: get from camera
    d->directionalLightShader->setUniform("colorSampler", d->gbuffer->colorSampler());
    d->directionalLightShader->setUniform("normalSampler", d->gbuffer->normalSampler());
    d->directionalLightShader->setUniform("positionSampler", d->gbuffer->positionSampler());
    for (int i = 0;  i < d->lights.size(); ++i) {
      if (d->lights.at(i)->type() == LT_DIRECTIONAL) {
        // set light parameters
        d->directionalLightShader->setUniform("direction", d->lights.at(i)->direction());
        d->directionalLightShader->setUniform("ambientColor", d->lights.at(i)->ambientColor());
        d->directionalLightShader->setUniform("diffuseColor", d->lights.at(i)->diffuseColor());
        d->directionalLightShader->setUniform("specularColor", d->lights.at(i)->specularColor());
        // render full screen quad
        d->quad->render();
      }
    }
    // deselect shader
    d->directionalLightShader->deselect();

//    d->gbuffer->blit();
  }
}
