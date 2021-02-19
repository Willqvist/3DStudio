//
// Created by William Lundqvist on 2020-11-02.
//
#include "Window.h"
#include "../platform/glfw/WindowGLFW.h"

std::shared_ptr<GLProj::Window> GLProj::Window::create(int width, int height,const std::string &title, bool vsync) {
    return std::make_shared<WindowGLFW>(width,height,title,vsync);
}

