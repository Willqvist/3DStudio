//
// Created by William Lundqvist on 2020-12-11.
//

#include <iostream>
#include "FBO.h"

GLProj::FBO::FBO(int width,int height) : width(width), height(height) {
    glGenFramebuffers(1,&fbo);
    glGenTextures(1, &mDepth);
    glBindTexture(GL_TEXTURE_2D, mDepth);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0,  GL_DEPTH_COMPONENT, GL_BYTE, nullptr);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepth, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete: " << fboStatus << std::endl;
}

void GLProj::FBO::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void GLProj::FBO::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint GLProj::FBO::getDepthBuffer() {
    return mDepth;
}

int GLProj::FBO::getWidth() const {
    return width;
}

int GLProj::FBO::getHeight() const {
    return height;
}
