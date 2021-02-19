//
// Created by William Lundqvist on 2020-11-12.
//

#include "Scene.h"
#include "../loaders/ObjLoader.h"
#include "../window/Input.h"
#include "../camera/Camera.h"
#include "../model/Model.h"
#include "../graphics/Renderer.h"
#include "../Core.h"
#include "layers/RenderingLayer.h"
#include "layers/InputLayer.h"
#include "layers/GUISettingsLayer.h"
#include "layers/GUIMaterialLayer.h"
#include "../tools/Tools.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>

GLProj::Scene::Scene(std::shared_ptr<Window> window): window(window) {

    std::shared_ptr<Camera> camera = createCamera();

    std::vector<GLProj::ShaderLocations> locations = {
            {"vPosition",0},
            {"vNormal",1},
            {"vTextureCoords",2},
            {"vColor",3},
    };
    std::shared_ptr<GLProj::Shader> shader = GLProj::Shader::create("res/vshader.glsl","res/fshader.glsl",locations);

    GLProj::ObjLoaderSettings settings = {
            true,
        true,
    };

    std::shared_ptr<GLProj::OBJ> obj = GLProj::ObjLoader::loadOBJ("res/cube.obj",settings);
    std::shared_ptr<GLProj::OBJ> floorObj = GLProj::ObjLoader::loadOBJ("res/cube.obj",settings);
    if(obj == nullptr || floorObj == nullptr) {
        exit(1);
    }

    std::shared_ptr<GLProj::Model> model = GLProj::Model::create(obj);
    std::shared_ptr<GLProj::Model> floorModel = GLProj::Model::create(floorObj);

    std::shared_ptr<GLProj::ModelInstance> instance = model->createInstance();
    std::shared_ptr<GLProj::ModelInstance> floor = floorModel->createInstance();
    std::shared_ptr<Texture> albedo = GLProj::Texture::load("res/bricks.bmp",GL_TEXTURE_2D);

    std::shared_ptr<Material> mat = std::make_shared<Material>(shader);

    mat->set("material.ambientReflection",glm::vec3(0.6,0.6,0.6));
    mat->set("material.diffuseReflection",glm::vec3(0.3,0.3,0.3));
    mat->set("material.specularReflection",glm::vec3(0.5,0.5,0.5));
    mat->set("material.shininess",255.0f);
    mat->set("albedo",albedo == nullptr ? Texture::white() : albedo);
    mat->set("useTextures",true);


    instance->setMaterial(mat,false);
    floor->setMaterial(mat);
    floor->getMaterial()->set("useTextures",false);

    floor->setScale(12,0.2,12);
    floor->setPosition(0,-1.5f,0);

    std::shared_ptr<RenderingLayer> renderingLayer = addLayer<RenderingLayer>(0, window);
    renderingLayer->addInstance(instance,"main");
    renderingLayer->addInstance(floor,"floor");


    std::shared_ptr<Light> mainLight = std::make_shared<Light>();
    mainLight->type = DIRECTIONAL;
    mainLight->direction = glm::vec3(-1,-1,-1);
    mainLight->castShadows = true;
    mainLight->color = glm::cvec3::one;
    mainLight->position = glm::vec3(6,6,6);
    renderingLayer->addLight(mainLight,"MainLight");

    std::shared_ptr<Light> light = std::make_shared<Light>();
    light->type = POINT;
    light->radius = 15;

    light->direction = glm::vec3(-1,-1,-1);
    light->color = glm::vec3(1,0,0);

    renderingLayer->addLight(light,"light");

    std::shared_ptr<Light> light2 = std::make_shared<Light>();
    light2->type = POINT;
    light2->radius = 15;

    light2->direction = glm::vec3(-1,-1,-1);
    light2->color = glm::vec3(0,1,0);
    light2->position = glm::vec3(-2,2,2);

    renderingLayer->addLight(light2,"light2");

    addLayer<InputLayer>(1,camera,instance,window);
    addLayer<GUISettingsLayer>(2, window, camera, renderingLayer);
    addLayer<GUIMaterialLayer>(3,camera, window,renderingLayer);
}

std::shared_ptr<GLProj::Camera> GLProj::Scene::createCamera() {
    std::shared_ptr<GLProj::Camera> camera = std::make_shared<GLProj::Camera>();

    std::shared_ptr<GLProj::PerspectiveProjection> proj = std::make_shared<GLProj::PerspectiveProjection>(
            70.0f, window->getWidth()/(window->getHeight()*1.0f), 0.01f, 1000.0f);

    camera->setProjection(proj);
    window->addListener(camera);

    camera->setPosition(0,0,2);
    camera->lookAt(0,0,0);
    camera->update();

    GLProj::Renderer::bindCamera(camera);
    return camera;
}

void GLProj::Scene::update() {
    for(auto& inst : layers) {
        inst->update();
    }
}