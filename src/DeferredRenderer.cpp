#include "DeferredRenderer.h"

#include "Attribute.h"
#include "Camera.h"
#include "CubeMesh.h"
#include "FragmentShader.h"
#include "GBuffer.h"
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
    DeferredRendererPrivate(uint width, uint height) : gbuffer(new GBuffer(width, height)) {
    }

    ~DeferredRendererPrivate() {
      delete gbuffer;
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

    GBuffer *gbuffer;
    Texture *texture;
    ShaderProgram *shaderProgram;
  };

  DeferredRenderer::DeferredRenderer(uint width, uint height) : Renderer(), d(new DeferredRendererPrivate(width, height)) {
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
    d->gbuffer->setReadable(true);
    // clear color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // blit the gbuffer to the screen
    d->gbuffer->blit();
    // unbind gbuffer
    d->gbuffer->setReadable(false);
  }
}
