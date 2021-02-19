//
// Created by William Lundqvist on 2020-11-10.
//

#include "Camera.h"
#include "../tools/Tools.h"
#include <glm/gtx/string_cast.hpp>
glm::mat4 GLProj::Camera::viewMatrix() {
    return viewMat;
}

glm::mat4 GLProj::Camera::projectionMatrix() {
    return projectionType->matrix();
}

void GLProj::Camera::update() {
    if(projectionType != nullptr)
        this->projectionType->update();
    viewMat = Tools::createViewMatrix(position,scale,rotation);
    if(projectionType != nullptr)
    viewProjMat = projectionType->matrix()*viewMat;

    glm::mat4 inv = glm::inverse(viewMat);
    forward = glm::normalize(glm::vec3(inv[2]));
    right = glm::normalize(glm::vec3(inv[0]));
    up = glm::normalize(glm::vec3(inv[1]));
}

void GLProj::Camera::onWindowResize(int width, int height) {
    if(projectionType != nullptr)
        projectionType->onResize(width,height);
    viewProjMat = projectionType->matrix()*viewMat;
}

void GLProj::Camera::setProjection(std::shared_ptr<Projection> projection) {
    projectionType = projection;
    viewProjMat = projectionType->matrix()*viewMat;
}

void GLProj::Camera::lookAt(float x, float y, float z) {
    glm::vec3 vec = glm::normalize(glm::vec3(x,y,z)-position);

    rotation.y = 90+glm::degrees(atan2(vec.z,-vec.x));
    rotation.x = 90+glm::degrees(atan2(vec.z < 0 ? vec.z : -vec.z,vec.y));

}
