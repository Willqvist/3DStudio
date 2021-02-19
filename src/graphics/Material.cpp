//
// Created by William Lundqvist on 2020-11-19.
//

#include "Material.h"

GLProj::Material::Material(std::shared_ptr<Shader> shader) : shader(shader) {
    visitor.shader = shader;
    std::shared_ptr<Texture> whiteTex = Texture::white();
    set("albedo",whiteTex);
    //set("bump",whiteTex);
    set("useTextures",true);
}

void GLProj::Material::setShader(std::shared_ptr<Shader> shader) {
    this->shader = shader;
    visitor.shader = shader;
}

void GLProj::Material::set(const std::string &name, std::shared_ptr<Texture> texture) {
    for(auto& tex : textures) {
        if(tex.name == name) {
            tex.texture = texture;
            return;
        }
    }
    this->textures.push_back({name,texture});
}

void GLProj::Material::bind() {
    shader->bind();
    for(auto& var : variables) {
        visitor.name = var.first;
        std::visit(visitor,var.second);
    }
    int slot = 0;
    for(MaterialTexture& tex : textures) {
        shader->setUniformi(tex.name,slot);
        tex.texture->bind(slot);
        slot ++;
    }
}

void GLProj::Material::unbind() {

}

std::shared_ptr<GLProj::Shader> GLProj::Material::getShader() {
    return shader;
}
