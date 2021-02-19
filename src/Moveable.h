//
// Created by William Lundqvist on 2020-11-13.
//

#ifndef GLPROJECT_MOVEABLE_H
#define GLPROJECT_MOVEABLE_H

#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>
#include "tools/cvec3.h"
/**
 * A class representing an object that can move, scale and rotate.
 */
class Moveable {
public:
    void setPosition(float x, float y, float z) {
        this->position = glm::vec3(x,y,z);
    }

    void setScale(float x, float y, float z) {
        this->scale = glm::vec3(x,y,z);
    }

    void setRotation(float x, float y, float z) {
        this->rotation = glm::vec3(x,y,z);
    }

    void translate(float x, float y, float z) {
        position.x += x;
        position.y += y;
        position.z += z;
    }

    void rotate(float x, float y, float z) {
        rotation.x += x;
        rotation.y += y;
        rotation.z += z;
    }

    glm::vec3& getPosition() {
        return position;
    }

    glm::vec3& getRotation() {
        return rotation;
    }

    glm::vec3& getScale() {
        return scale;
    }

    glm::vec3 getUp() {
        return up;
    }

    glm::vec3 getForward() {
        return forward;
    }
    glm::vec3 getRight() {
        return right;
    }

protected:
    glm::vec3 position = glm::vec3(0,0,0);
    glm::vec3 rotation = glm::vec3(0,0,0);
    glm::vec3 scale = glm::vec3(1,1,1);
    glm::vec3 forward = glm::vec3(0,0,-1);
    glm::vec3 up = glm::vec3 (0,1,0);
    glm::vec3 right = glm::vec3 (1,0,0);
};
#endif //GLPROJECT_MOVEABLE_H
