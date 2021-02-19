//
// Created by William Lundqvist on 2020-12-05.
//

#include "RenderingLayer.h"
#include "../../graphics/Renderer.h"
GLProj::RenderingLayer::RenderingLayer(std::shared_ptr<Window>& window) : window(window) {

}

void GLProj::RenderingLayer::init() {

}

void GLProj::RenderingLayer::update() {
    GLProj::Renderer::renderShadowMaps(window, instances,lights);
    for(auto& inst : instances) {
        GLProj::Renderer::render(mode, inst.instance,lights);
    }
}

GLProj::RenderingLayer::~RenderingLayer() {

}

void GLProj::RenderingLayer::addInstance(std::shared_ptr<ModelInstance> instance, std::string name) {
    if(name.length() > 0 )
        instanceMap[name] = {name,instance};
    instances.push_back({name,instance});
}

void GLProj::RenderingLayer::addLight(std::shared_ptr<Light> instance, std::string name) {
    if(name.length() > 0 )
        lightsMap[name] = {name,instance};
    lights.push_back({name,instance});
}

void GLProj::RenderingLayer::setMode(GLuint mode) {
    this->mode = mode;
}
