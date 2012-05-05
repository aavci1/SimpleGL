#include "Renderer.h"

#include "Camera.h"
#include "DirectionalLight.h"
#include "GBuffer.h"
#include "Light.h"
#include "Material.h"
#include "MaterialManager.h"
#include "Mesh.h"
#include "MeshManager.h"
#include "SceneNode.h"
#include "PointLight.h"
#include "Program.h"
#include "Sphere.h"
#include "SubMesh.h"
#include "Util.h"

#include <GL/glew.h>

#include <FreeImage.h>

#include <algorithm>

namespace SimpleGL {
  class RendererPrivate {
  public:
    RendererPrivate(uint width, uint height) : width(width), height(height), gbuffer(new GBuffer(width, height)) {
    }

    ~RendererPrivate() {
      delete gbuffer;
    }

    void renderHelper(Camera *camera, SceneNode *node, glm::mat4 modelMatrix, glm::mat4 viewProjMatrix) {
      modelMatrix *= node->transformationMatrix();
      // visit child nodes
      for (uint i = 0; i < node->nodes().size(); ++i)
        renderHelper(camera, node->nodes().at(i), modelMatrix, viewProjMatrix);
      // add lights to the list
      for (uint i = 0; i < node->lights().size(); ++i)
        if (node->lights().at(i)->isVisibleFrom(camera))
          lights.push_back(node->lights().at(i));
      // render meshes
      for (uint i = 0; i < node->meshes().size(); ++i) {
        Mesh *mesh = node->meshes().at(i);
        if (!mesh->isVisibleFrom(camera))
          continue;
        for (uint j = 0; j < mesh->subMeshes().size(); ++j) {
          SubMesh *subMesh = mesh->subMeshes().at(j);
          Material *material = MaterialManager::instance()->getMaterialByName(subMesh->materialName());
          if (!material || !material->program())
            continue;
          // select program
          material->select();
          // update uniforms
          material->program()->setUniform("sglModelMatrix", modelMatrix);
          material->program()->setUniform("sglModelViewProjMatrix", viewProjMatrix * modelMatrix);
          // render the mesh
          subMesh->render();
          // deselect shader
          material->deselect();
        }
      }
    }

    int width;
    int height;
    GBuffer *gbuffer;
    std::vector<Light *> lights;
  };

  Renderer::Renderer(uint width, uint height) : d(new RendererPrivate(width, height)) {
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

  void Renderer::renderOneFrame(Camera *camera, SceneNode *root) {
    // clear color buffer
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // set general parameters
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    // set parameters for the geometry pass
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    // GEOMETRY PASS
    // bind gbuffer for writing
    d->gbuffer->setWritable(true);
    // clear color and depth buffers
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // clear lights
    d->lights.clear();
    glm::mat4 viewProjMatrix = camera->projectionMatrix() * camera->viewMatrix();
    // render scene
    d->renderHelper(camera, root, glm::mat4(), viewProjMatrix);
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
#if 0
    // blit gbuffer to the screen
    d->gbuffer->blit();
#else
    for (int type = LT_DIRECTIONAL; type <= LT_SPOT; ++type) {
      Material *material = MaterialManager::instance()->getMaterialByLightType(LightType(type));
      if (!material || !material->program())
        continue;
      material->program()->bind();
      material->program()->setUniform("colorBuffer", d->gbuffer->colorBuffer());
      material->program()->setUniform("normalBuffer", d->gbuffer->normalBuffer());
      material->program()->setUniform("positionBuffer", d->gbuffer->positionBuffer());
      material->program()->setUniform("screenSize", glm::vec2(d->width, d->height));
      material->program()->setUniform("cameraPos", camera->position());
      // render the light
      for (uint i = 0; i < d->lights.size(); ++i)
        if (d->lights.at(i)->type() == type)
          d->lights.at(i)->render(camera);
      // deselect material
      material->program()->unbind();
    }
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
