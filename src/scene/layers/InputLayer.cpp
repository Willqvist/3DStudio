//
// Created by William Lundqvist on 2020-12-05.
//

#include "InputLayer.h"
#include "../../window/Input.h"
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>

GLProj::InputLayer::InputLayer(std::shared_ptr<Camera> camera, std::shared_ptr<ModelInstance> instance,
        std::shared_ptr<Window> window) : camera(camera),instance(instance), window(window) {

}

void GLProj::InputLayer::init() {

}

void GLProj::InputLayer::update() {
    const float mouseRotationSpeed = 0.05f, moveSpeed = 0.05f;

    if(ImGui::IsAnyItemActive()) {
        this->window->mouseEnabled(true);
        return;
    }

    if(GLProj::Input::isKeyDown(GLFW_KEY_UP)) {
        instance->rotate(-6.0f, 0, 0);
    }

    if(GLProj::Input::isKeyDown(GLFW_KEY_DOWN)) {
        instance->rotate(6.0f, 0, 0);
    }

    if(GLProj::Input::isKeyDown(GLFW_KEY_RIGHT)) {
        instance->rotate(0, 0, 6.0f);
    }

    if(GLProj::Input::isKeyDown(GLFW_KEY_LEFT)) {
        instance->rotate(0, 0, -6.0f);
    }

    if(GLProj::Input::isKeyDown(GLFW_KEY_I)) {
        instance->translate(0, 0.1f, 0);
    }

    if(GLProj::Input::isKeyDown(GLFW_KEY_K)) {
        instance->translate(0, -0.1f, 0);
    }

    if(GLProj::Input::isKeyDown(GLFW_KEY_J)) {
        instance->translate(-0.1f, 0, 0);
    }

    if(GLProj::Input::isKeyDown(GLFW_KEY_L)) {
        instance->translate(0.1f, 0, 0);
    }

    if(GLProj::Input::isKeyDown(GLFW_KEY_N)) {
        instance->translate(0, 0, 0.1f);
    }

    if(GLProj::Input::isKeyDown(GLFW_KEY_M)) {
        instance->translate(0, 0, -0.1f);
    }

    //camera movement
    if(GLProj::Input::isKeyDown(GLFW_KEY_E)) {
        glm::vec3 up = camera->getUp()*moveSpeed;
        camera->translate(up.x,up.y,up.z);
        camera->update();
    }
    if(GLProj::Input::isKeyDown(GLFW_KEY_Q)) {
        glm::vec3 up = -camera->getUp()*moveSpeed;
        camera->translate(up.x,up.y,up.z);
        camera->update();
    }
    if(GLProj::Input::isKeyDown(GLFW_KEY_W)) {
        glm::vec3 forward = -camera->getForward()*moveSpeed;
        camera->translate(forward.x,forward.y,forward.z);
        camera->update();
    }
    if(GLProj::Input::isKeyDown(GLFW_KEY_S)) {
        glm::vec3 forward = camera->getForward()*moveSpeed;
        camera->translate(forward.x,forward.y,forward.z);
        camera->update();
    }
    if(GLProj::Input::isKeyDown(GLFW_KEY_A)) {
        glm::vec3 right = -camera->getRight()*moveSpeed;
        camera->translate(right.x,right.y,right.z);
        camera->update();
    }
    if(GLProj::Input::isKeyDown(GLFW_KEY_D)) {
        glm::vec3 right = camera->getRight()*moveSpeed;
        camera->translate(right.x,right.y,right.z);
        camera->update();
    }

    if(GLProj::Input::isMouseDown(GLFW_MOUSE_BUTTON_1)) {
        glm::vec2 deltaMouse = Input::deltaMouse()*mouseRotationSpeed;
        camera->rotate(-deltaMouse.y,-deltaMouse.x,0);
        camera->update();
    }

    if(GLProj::Input::isMousePressed(GLFW_MOUSE_BUTTON_1)) {
        this->window->mouseEnabled(false);
    }

    if(GLProj::Input::isMouseRelease(GLFW_MOUSE_BUTTON_1)) {
        this->window->mouseEnabled(true);
    }
}

GLProj::InputLayer::~InputLayer() {

}
