//
// Created by William Lundqvist on 2020-11-06.
//

#ifndef GLPROJECT_GLFWWINDOWEVENTHANDLER_H
#define GLPROJECT_GLFWWINDOWEVENTHANDLER_H

#include <GLFW/glfw3.h>
#include "../../window/Window.h"
#include "WindowGLFW.h"

namespace GLProj {
    class WindowGLFW;

    /**
     * event handler for GLFW window.
     */
    class GLFWWindowEventHandler {
        friend class WindowGLFW;
    private:

        /*
         * Method callback when a key is pressed
         */
        static void OnKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode) {
            WindowGLFW* ptr = static_cast<WindowGLFW*>(glfwGetWindowUserPointer(window));
            for(auto listener : ptr->listeners) {
                listener->onKey(key,action);
            }
        }
        /*
         * Method callback when a mouse button has been pressed
         */
        static void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
            WindowGLFW* ptr = static_cast<WindowGLFW*>(glfwGetWindowUserPointer(window));

            for(auto listener : ptr->listeners) {
                listener->onMousePress(button,action);
            }
        }

        /*
         * Method callback for when a cusor position has changed
         */
        static void CursorPosCallback(GLFWwindow *window, double x, double y) {
            WindowGLFW* ptr = static_cast<WindowGLFW*>(glfwGetWindowUserPointer(window));
            for(auto listener : ptr->listeners) {
                listener->onMouseMove(x,y);
            }
        }

        /*
         * method callback when the window size has changed.
         */
        static void WindowSizeCallback(GLFWwindow *window, int width, int height) {
            WindowGLFW* ptr = static_cast<WindowGLFW*>(glfwGetWindowUserPointer(window));
            ptr->resize(width,height);
            for(auto listener : ptr->listeners) {
                listener->onWindowResize(width,height);
            }
        }
    };
}
#endif //GLPROJECT_GLFWWINDOWEVENTHANDLER_H
