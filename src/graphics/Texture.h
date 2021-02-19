//
// Created by William Lundqvist on 2020-11-17.
//

#ifndef GLPROJECT_TEXTURE_H
#define GLPROJECT_TEXTURE_H


#include <string>
#include <glad/glad.h>
#include <memory>
#define TEXTURE_BUFFER_SIZE 255
namespace GLProj {
    class TextureLoader;

    /**
     * abstractr class representing a texture.
     */
    class Texture {
    public:

        static std::shared_ptr<Texture> load(const std::string &path, GLenum type);

        /**
         * binds the texture to the graphics card.
         * @param slot what slot to bind to.
         */
        void bind(int slot);

        /**
         * unbinds the texture.
         */
        void unbind();

        /**
         * the path to the texture.
         * @return
         */
        std::string& getPath() {
            return path;
        }

        char* getPathBuffer() {
            return pathBuff;
        }

        /**
         * creates a 1x1 white texture.
         */
        static std::shared_ptr<Texture> white();

        GLuint getId();

    private:
        Texture() = default;
        std::string path;
        char pathBuff[TEXTURE_BUFFER_SIZE];
        GLuint id;
        GLenum type;
        static inline std::shared_ptr<Texture> whiteTexture;
        friend class TextureLoader;
    };
}


#endif //GLPROJECT_TEXTURE_H
