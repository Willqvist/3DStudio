//
// Created by William Lundqvist on 2020-11-13.
//

#ifndef GLPROJECT_PROJECTION_H
#define GLPROJECT_PROJECTION_H

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include "../Core.h"

namespace GLProj {

    /**
     * class used in Camera to store and represent different type of projections
     */
    class Projection {
    public:
        //updates the projection
        virtual void update() = 0;

        //is called when window resize event has occured.
        virtual void onResize(int width, int height) = 0;

        //returns the projection matrix.
        glm::mat4 &matrix() {
            return mat;
        }

        virtual ~Projection() = default;

    protected:
        glm::mat4 mat;
    };

    /**
     * perspective projection.
     */
    class PerspectiveProjection : public Projection {
    public:

        /**
         *
         * @param fov field of view for the camera
         * @param aspect aspecty ratio for the camera
         * @param near near plane for the camera
         * @param far far place for the camera
         */
        PerspectiveProjection(float fov, float aspect, float near, float far) : fov(fov), near(near),
                                                                                         far(far), aspect(aspect) {
            mat = glm::perspective(fov, aspect, near, far);
        }

        void update() override {
            mat = glm::perspective(fov, aspect, near, far);
        }

        void onResize(int width, int height) override {
            if(width == 0 || height == 0) return;
            aspect = width / (height * 1.0f);
            mat = glm::perspective(fov, aspect, near, far);
        }

        ~PerspectiveProjection() = default;

        //sets the field of view
        void setFov(float fov) {
            this->fov = fov;
        }

        //sets the near values
        void setNear(float near) {
            this->near = near;
        }

        //sets the far
        void setFar(float far) {
            this->far = far;
        }

    private:
        float fov, near, far,aspect;
    };

    /**
     * parallel projection, both ortrhographic and oblique.
     */
    class ParallelProjection : public Projection {
    public:

        /**
         *
         * @param width width of the projection
         * @param height height of the projection
         * @param top top value of the far plane
         * @param zNear near plane for the camera
         * @param zFar far plane for the camera
         */
        ParallelProjection(float width, float height, float top, float zNear, float zFar) : top(top),bottom(-top),zNear(zNear),zFar(zFar),scale(0),angle(45.0f){
            onResize(width,height);
        }

        void update() override {
            setHMatrix();
            mat = ST*H;
            //mat = glm::ortho(left,right,top,bottom,zNear,zFar);
        }

        void onResize(int width, int height) override {
            this->width = width;
            this->height = height;
            float aspect = width / (height * 1.0f);
            float frustumHeight = glm::abs(top - bottom);
            float frustumWidth = frustumHeight*aspect;
            left = -frustumWidth/2.0f;
            right = -left;
            update();
        }

        ~ParallelProjection() = default;

        //set top value for the projection
        void setTop(float top) {
            this->top = top;
            this->bottom = -top;
            onResize(this->width,this->height);
        }

        //sets the scale.
        void setScale(float scale) {
            this->scale = scale;
        }

        //sets the oblique angle.
        void setAngle(float angle) {
            this->angle = angle;
        }

        //sets the far value.
        void setFar(float far) {
            this->zFar = far;
        }

    private:

        //creates the H matrix.
        void setHMatrix() {
            H = glm::mat4(1.0f);
            H[2][0] = scale*cos(glm::radians(angle));
            H[2][1] = scale*sin(glm::radians(angle));

            ST = glm::ortho(left,right,bottom,top,zNear,zFar);
        }
        glm::mat4 H = glm::mat4(1), ST = glm::mat4(1.0f);
        float left, right, top, bottom, zNear, zFar, scale, angle;
        int width,height;
    };
}
#endif //GLPROJECT_PROJECTION_H
