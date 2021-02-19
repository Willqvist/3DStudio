//
// Created by William Lundqvist on 2020-11-19.
//

#ifndef GLPROJECT_TEXTURELOADER_H
#define GLPROJECT_TEXTURELOADER_H

#include <glad/glad.h>
#include <map>
#include "../graphics/Texture.h"

namespace GLProj {

    /**
     * loader class that loads and creates textures, it also caches previously loaded textures.
     */
    class TextureLoader {
    public:

        /**
         * loads a texture with a given path, will cache it so if same file is loaded it will return same memory address
         * as first loaded file.
         * @param path the path to the texture
         * @param type the type of the image
         * @return shared pointer of a Texture.
         */
        static std::shared_ptr<Texture> load(const std::string &path, GLenum type);

        /**
         * loads a texture from given data and width/height and number of channels. This will not cache.
         * @param data the data to turn into a texture
         * @param width the width if the texture
         * @param height the height of the texture
         * @param nrChannels number of channels
         * @param type the tpe of the image
         * @return a Texture.
         */
        static std::shared_ptr<Texture>
        loadFromData(unsigned char *data, int width, int height, int nrChannels, GLenum type);

    private:
        static inline std::map<std::string, std::shared_ptr<Texture>> cache;
    };
}


#endif //GLPROJECT_TEXTURELOADER_H