//
// Created by William Lundqvist on 2020-12-11.
//

#ifndef GLPROJECT_FBO_H
#define GLPROJECT_FBO_H


#include <glad/glad.h>
namespace GLProj {

    /**
     * FBO(Framebuffer object) used when rendering should be dont but not to a window. f.ex. creating a depthmap for a light.
     */
    class FBO {
    public:
        /**
         * creates a new FBO width a given width and height.
         * @param width the width of the FBO
         * @param height the height of the FBO
         */
        FBO(int width,int height);

        /**
         * binds the FBO
         */
        void bind();

        /**
         * unbinds the FBO
         */
        void unbind();

        /**
         * returns the id to the depthbuffer texture.
         */
        GLuint getDepthBuffer();

        /**
         * @return the width of the fbo
         */
        int getWidth() const;

        /**
         * @return the height of the FBO.
         */
        int getHeight() const;

    private:
        GLuint fbo,mDepth,mCol;
        int width,height;
    };
}


#endif //GLPROJECT_FBO_H
