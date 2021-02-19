//
// Created by William Lundqvist on 2020-11-06.
//

#include "Renderer.h"
#include "../tools/Tools.h"
#include "../Core.h"
#include "../model/Model.h"
#include "Light.h"
#include "../scene/layers/RenderingLayer.h"

void GLProj::Renderer::render(GLuint drawMode, std::shared_ptr<ModelInstance> instance, std::vector<LightInstance> lights) {
    if(instance->parent == nullptr) return;
    glm::mat4 model = Tools::createModelMatrix(instance->position,instance->scale,instance->rotation);
    for(Mesh& mesh : instance->getModel()->meshes()) {
        mesh.bind();
        std::shared_ptr<Material> material;
        if(instance->materials.find(mesh.vao) != instance->materials.end()) {
            material = instance->materials[mesh.vao];
            material->bind();

            std::shared_ptr<Shader> shader = material->getShader();
            shader->setUniform("ModelMatrix", model);
            shader->setUniform("time",Tools::timeSinceStart());
            shader->setUniform("ambientStrength",0.8f);
            shader->setUniform("ambientLightColor",glm::vec3(1,1,1));
            shader->setUniform("cameraPosition",boundCamera->getPosition());
            shader->setUniformi("hasTexCoords",instance->getModel()->hasTextureCoords());
            int lightIndex = 0;
            hasFoundDirectionalLight = false;
            for(auto& light : lights) {
                bindLight(light.instance,lightIndex,shader);
                lightIndex ++;
            }
            shader->setUniformi("numOfLights",lightIndex);

            if(boundCamera) {
                shader->setUniform("ViewMatrix",boundCamera->viewMatrix());
                shader->setUniform("ProjectionMatrix", boundCamera->projectionMatrix());
            }
            else {
                shader->setUniform("ViewMatrix", glm::mat4(1.0f));
                shader->setUniform("ProjectionMatrix", glm::mat4(1.0f));
            }
        }
        glDrawElements(drawMode, mesh.indexSize, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
        if(material)
            material->unbind();
    }
}

void GLProj::Renderer::bindCamera(std::shared_ptr<Camera>& camera) {
   boundCamera = camera;
}

void GLProj::Renderer::bindLight(std::shared_ptr<Light> light, int index, std::shared_ptr<Shader> shader) {
    std::ostringstream oss;
    oss << "lights[" << index << "]";
    std::string name = oss.str();
    shader->setUniform(name + ".position", light->position);
    shader->setUniform(name + ".direction", light->direction);
    shader->setUniform(name + ".shadow", light->castShadows);
    shader->setUniform(name + ".color", light->color);
    shader->setUniform(name + ".radius", 1.0f/light->radius);
    shader->setUniform(name + ".lightSpaceMatrix", light->spaceMatrix);

    if(light->castShadows) {
        glActiveTexture(GL_TEXTURE0 + 10);
        glBindTexture(GL_TEXTURE_2D,light->depthTexture);
        shader->setUniformi("ShadowMap",10);
    }

    if(light->type == DIRECTIONAL) {
        if(hasFoundDirectionalLight) {
            DEBUGERR("Only one directional light per scene!");
            exit(1);
        }
        hasFoundDirectionalLight = true;
    }
}

void GLProj::Renderer::clear() {
    hasFoundDirectionalLight = false;
}

void GLProj::Renderer::renderShadowMaps(std::shared_ptr<Window>& window, std::vector<RenderingInstance>& instances, std::vector<LightInstance> lights) {
    glCullFace(GL_FRONT);
    glViewport(0, 0, shadowMapFbo->getWidth(), shadowMapFbo->getHeight());
    shadowMapFbo->bind();
    glClear(GL_DEPTH_BUFFER_BIT);
    shadowShader->bind();
    for(auto& light : lights) {
        auto& l = light.instance;
        if(!light.instance->castShadows) continue;
        shadowMapCamera->setPosition(l->position.x,l->position.y,l->position.z);
        glm::vec3 lookP = l->position + l->direction;
        shadowMapCamera->lookAt(lookP.x,lookP.y,lookP.z);
        shadowMapCamera->update();

        shadowShader->setUniform("ViewMatrix",shadowMapCamera->viewMatrix());
        shadowShader->setUniform("ProjectionMatrix", projection);

        for(auto& inst : instances) {
            auto& instance = inst.instance;
            if(instance->parent == nullptr)
                continue;
            glm::mat4 model = Tools::createModelMatrix(instance->position,instance->scale,instance->rotation);
            shadowShader->setUniform("ModelMatrix", model);
            for (Mesh &mesh : instance->getModel()->meshes()) {
                mesh.bind();

                glDrawElements(GL_TRIANGLES, mesh.indexSize, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
            }
        }
        l->spaceMatrix = projection*shadowMapCamera->viewMatrix();
        l->depthTexture = shadowMapFbo->getDepthBuffer();
    }
    shadowMapFbo->unbind();
    glViewport(0, 0, window->getWidth(), window->getHeight());
    glCullFace(GL_BACK);
}


void GLProj::Renderer::init(std::shared_ptr<Window>& window) {
    shadowMapFbo = std::make_shared<FBO>(2000,2000);
    shadowMapCamera = std::make_shared<Camera>();
    projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 5.0f,25.0f);
    std::vector<GLProj::ShaderLocations> locations = {
            {"vPosition",0},
            {"vNormal",1},
            {"vTextureCoords",2},
            {"vColor",3}};
     shadowShader = Shader::create("res/vshadow.glsl","res/fshadow.glsl",locations);

}
