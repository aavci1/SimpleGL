#include "Renderer.h"

#include "Attribute.h"
#include "Camera.h"
#include "FragmentShader.h"
#include "GBuffer.h"
#include "Light.h"
#include "Node.h"
#include "Quad.h"
#include "Program.h"
#include "Texture.h"
#include "VertexShader.h"

#include <GL/glew.h>

#include <fstream>
#include <iostream>
#include <sstream>

namespace SimpleGL {
  class RendererPrivate {
  public:
    RendererPrivate(uint width, uint height) : width(width), height(height), gbuffer(new GBuffer(width, height)), directionalLightProgram(0), quad(new Quad()) {
    }

    ~RendererPrivate() {
      delete gbuffer;
      delete texture;
      delete geometryProgram;
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
    Program *directionalLightProgram;
    Quad *quad;
    Texture *texture;
    Program *geometryProgram;
  };

  Renderer::Renderer(uint width, uint height) : d(new RendererPrivate(width, height)) {
    // load directional light shader
    d->directionalLightProgram = new Program();
    d->directionalLightProgram->addShader(new VertexShader(d->readAll("media/deferred_directional_light_vp.glsl")));
    d->directionalLightProgram->addShader(new FragmentShader(d->readAll("media/deferred_directional_light_fp.glsl")));
    if (!d->directionalLightProgram->compileAndLink())
      printf("error: can not compile shader:\n%s", d->directionalLightProgram->message().c_str());
    // load texture
    d->texture = new Texture("media/laminate.jpg");
    if (!d->texture->load())
      printf("error: can not load texture %s.", d->texture->path().c_str());
    // load shader
    d->geometryProgram = new Program();
    d->geometryProgram->addShader(new VertexShader(d->readAll("media/deferred_vp.glsl")));
    d->geometryProgram->addShader(new FragmentShader(d->readAll("media/deferred_fp.glsl")));
    if (!d->geometryProgram->compileAndLink())
      printf("error: can not compile shader:\n%s", d->geometryProgram->message().c_str());
  }

  Renderer::~Renderer() {
    delete d;
  }

  void Renderer::renderOneFrame(Camera *camera, Node *root) {
    // GEOMETRY PASS
    // bind gbuffer for writing
    d->gbuffer->setWritable(true);
    // set general parameters
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);
    // set parameters for the geometry pass
    glDepthMask(GL_TRUE);
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
    // TODO: points lights pass
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
//    // clear color buffer
//    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//    glClear(GL_COLOR_BUFFER_BIT);
//    // blit gbuffer to the screen
//    d->gbuffer->blit();
  }
}
