//
// Created by William Lundqvist on 2020-11-02.
//

#ifndef GLPROJECT_WINDOWGLFW_H
#define GLPROJECT_WINDOWGLFW_H


#include "../../window/Window.h"
#include <GLFW/glfw3.h>
namespace GLProj {
    class GLFWWindowEventHandler;

    /**
     * window instance using GLFW as underlying API. See comments in Window for general documentation.
     */
    class WindowGLFW : public Window {
    public:
        WindowGLFW(int width, int height, const std::string &title, bool vsync);

        bool update() override;

        void clear() override;

        void dispose() override;

        void mouseEnabled(bool enabled) override;

        ~WindowGLFW() override = default;

    private:
        /**
         * setups up GLFW listeners.
         */
        void setupListeners();
        GLFWwindow *window;
        friend class GLFWWindowEventHandler;

        void resize(int width,int height);
    };
}


#endif //GLPROJECT_WINDOWGLFW_H
