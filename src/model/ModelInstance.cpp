//
// Created by William Lundqvist on 2020-11-05.
//

#include <glad/glad.h>
#include "Model.h"
#include "../Core.h"
#include "ModelInstance.h"
#include "Model.h"
void GLProj::ModelInstance::setMaterial(std::shared_ptr<Material> mat,int index,bool copy) {
    std::shared_ptr<Material> material;
    if(copy)
        material = std::make_shared<Material>(*mat);
    else
        material = mat;
    if(index < 0) {
        this->material = material;
        for (auto &m : parent->subMeshes) {
            materials[m.vao] = material;
        }
    } else {

        if(parent->subMeshes.size() <= index) return;
        Mesh& mesh = parent->subMeshes[index];
        materials[mesh.vao] = material;
    }
}

void GLProj::ModelInstance::setMaterial(std::shared_ptr<Material> material, bool copy) {
    this->setMaterial(material,-1,copy);
}

void GLProj::ModelInstance::setModel(std::shared_ptr<Model> model) {
    parent = model;
    if(material) {
        for(auto& m : parent->subMeshes) {
            materials[m.vao] = material;
        }
    }
}

std::shared_ptr<GLProj::Model> GLProj::ModelInstance::getModel() {
    return parent;
}

GLProj::Mesh &GLProj::ModelInstance::mesh(int index)  {
    return parent->meshes()[index];
}

std::shared_ptr<GLProj::Material> GLProj::ModelInstance::getMaterial() {
    return material;
}