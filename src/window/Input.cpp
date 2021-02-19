//
// Created by William Lundqvist on 2020-11-10.
//
#include "Input.h"
#include <GLFW/glfw3.h>

glm::vec2 GLProj::Input::mousePos = glm::vec2(0,0);
glm::vec2 GLProj::Input::prevMousePos = glm::vec2(0,0);
GLProj::KeyState GLProj::Input::mouseKeys[MOUSE_SIZE];
GLProj::KeyState GLProj::Input::keys[KEY_SIZE];

void GLProj::Input::onKey(int key, int state) {
    keys[key].down = state == GLFW_PRESS || state == GLFW_REPEAT;
    keys[key].pressed = state == GLFW_PRESS;
}

void GLProj::Input::onMouseMove(int x, int y) {
    prevMousePos = mousePos;
    mousePos = glm::vec2(x,y);
}

void GLProj::Input::onMousePress(int key, int state) {
    mouseKeys[key].down = state == GLFW_PRESS || state == GLFW_REPEAT;
    mouseKeys[key].pressed = state == GLFW_PRESS;
    mouseKeys[key].released = state == GLFW_RELEASE;
}

void GLProj::Input::onClear() {
    for(int i = 0; i < KEY_SIZE; i++) {
        keys[i].pressed = false;
    }
    for(int i = 0; i < MOUSE_SIZE; i++) {
        mouseKeys[i].pressed = false;
        mouseKeys[i].released = false;
    }

    prevMousePos = mousePos;
}

bool GLProj::Input::isKeyDown(int key) {
    return keys[key].down;
}

bool GLProj::Input::isMouseDown(int mouse) {
    return mouseKeys[mouse].down;
}

void GLProj::Input::onWindowResize(int width, int height) {

}

glm::vec2 GLProj::Input::deltaMouse() {
    return mousePos-prevMousePos;
}

bool GLProj::Input::isMouseRelease(int mouse) {
    return mouseKeys[mouse].released;
}

bool GLProj::Input::isMousePressed(int mouse) {
    return mouseKeys[mouse].pressed;
}
