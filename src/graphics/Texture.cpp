//
// Created by William Lundqvist on 2020-11-17.
//

#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "../loaders/TextureLoader.h"

void GLProj::Texture::bind(int slot) {
    glActiveTexture(GL_TEXTURE0+slot);
    glBindTexture(type,id);
}

void GLProj::Texture::unbind() {
    glBindTexture(type,0);
}

std::shared_ptr<GLProj::Texture> GLProj::Texture::load(const std::string &path, GLenum type) {
    return TextureLoader::load(path,type);
}

std::shared_ptr<GLProj::Texture> GLProj::Texture::white() {
    if(whiteTexture != nullptr)
        return whiteTexture;
    unsigned char data[4] = {255,255,255,255};
    whiteTexture = TextureLoader::loadFromData(data,1,1,4,GL_TEXTURE_2D);
    return whiteTexture;
}

GLuint GLProj::Texture::getId() {
    return id;
}
