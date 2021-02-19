//
// Created by William Lundqvist on 2020-11-10.
//

#ifndef GLPROJECT_CAMERA_H
#define GLPROJECT_CAMERA_H

#include <glm/mat4x4.hpp>
#include "../Moveable.h"
#include "../window/Window.h"
#include "Projection.h"

namespace GLProj {

    /**
     * class representing a camera in the world.
     */
    class Camera: public Moveable,public WindowEventListenerAdapter {
    public:

        //updates the camera.
        void update();

        //sets the projection type of this camera.
        void setProjection(std::shared_ptr<Projection> projection);

        //returns the projection of this camera.
        template<class T>
        std::shared_ptr<T> getProjection() {
            return std::dynamic_pointer_cast<T>(projectionType);
        }

        //changes the rotation of the camera to look at a point.
        void lookAt(float x,float y,float z);

        //returns the view matrix of this camera
        glm::mat4 viewMatrix();

        //returns the projection amtrix of this camera. same as getPorjection().matrix()
        glm::mat4 projectionMatrix();

        //is called when the window has resize.
        void onWindowResize(int width, int height);
    private:
        glm::mat4 viewMat = glm::mat4(1.0f), viewProjMat;
        std::shared_ptr<Projection> projectionType;
    };
}


#endif //GLPROJECT_CAMERA_H
