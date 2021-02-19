//
// Created by William Lundqvist on 2020-11-19.
//

#include <stb_image.h>
#include "TextureLoader.h"

std::shared_ptr<GLProj::Texture> GLProj::TextureLoader::load(const std::string &path, GLenum type) {
    if(cache.find(path) != cache.end()) {
        return cache[path];
    }

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(1);
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if(data == NULL) {
        fprintf(stderr,"Could not load image: %s \n",path.c_str());
        return nullptr;
    }
    cache[path] = loadFromData(data,width,height,nrChannels,type);
    cache[path]->path = path;
    strncpy(cache[path]->pathBuff, path.c_str(), sizeof(cache[path]->pathBuff) - 1);
    stbi_image_free(data);
    return cache[path];
}

std::shared_ptr<GLProj::Texture> GLProj::TextureLoader::loadFromData(unsigned char* data,int width,int height,int nrChannels, GLenum type) {
    GLuint textureId;
    glGenTextures(1,&textureId);
    glBindTexture(type, textureId);

    glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);

    GLuint chnls = nrChannels == 4 ? GL_RGBA : GL_RGB;
    glTexImage2D(type, 0, chnls, width, height, 0, chnls, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(type);

    Texture* tex = new Texture();
    tex->id = textureId;
    tex->type = type;
    return std::shared_ptr<Texture>(tex);
}
