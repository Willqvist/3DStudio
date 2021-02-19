//
// Created by William Lundqvist on 2020-11-06.
//

#ifndef GLPROJECT_SHADER_H
#define GLPROJECT_SHADER_H

#include <string>
#include <glm/mat4x4.hpp>
#include <memory>
#include <map>
#include <vector>
#include "Texture.h"

namespace GLProj {

    /**
     * struct representing a texture for a shader, its name and slot in the graphics card.
     */
    struct ShaderTexture {
        std::shared_ptr<Texture> texture;
        GLint slot;
        std::string name;
    };

    /**
     * a struct the contains the name and location for a attribute.
     */
    struct ShaderLocations {
        std::string name;
        int location;
    };

    /**
     * abstract class for a Shader.
     */
    class Shader {
    public:
        Shader(const std::string &vertexShader, const std::string &fragmentShader,std::vector<ShaderLocations> locations);

        /**
         * binds the shader to be used.
         */
        virtual void bind() = 0;

        /**
         * unbinds it so no shader is bound after this.
         */
        virtual void unbind() = 0;

        /**
         * sets a uniform value for name with the matrix 4x4
         * @param name name of the uniform
         * @param value the value to set.
         */
        virtual void setUniform(const std::string &name, glm::mat4 value) = 0;

        /**
         * sets a uniform value for name with the vector3
         * @param name name of the uniform
         * @param value the value to set.
         */
        virtual void setUniform(const std::string &name, glm::vec3 value) = 0;

        /**
         * sets a uniform value for name with the vector2
         * @param name name of the uniform
         * @param value the value to set.
         */
        virtual void setUniform(const std::string &name, glm::vec2 value) = 0;

        /**
         * sets a uniform value for name with the vector4
         * @param name name of the uniform
         * @param value the value to set.
         */
        virtual void setUniform(const std::string &name, glm::vec4 value) = 0;

        /**
         * sets a uniform value for name with the float
         * @param name name of the uniform
         * @param value the value to set.
         */
        virtual void setUniform(const std::string &name, float value) = 0;

        /**
         * sets a uniform value for name with the int
         * @param name name of the uniform
         * @param value the value to set.
         */
        virtual void setUniformi(const std::string &name, int value) = 0;

        /**
         * binds a id to a name
         * @param id  the id
         * @param name the name
         */
        void set(int id, const std::string& name);

        /**
         * gets the name bound to a given id. set in method set()
         * @param id the id the name is bound to.
         * @return the name
         */
        const std::string& get(int id);

        /**
         * destroys the shader and frees memory.
         */
        virtual void destroy() = 0;

        /**
         * creates a new shader
         * @param vertexShader the vertex shader
         * @param fragmentShader the fragment shader
         * @param locations attribure locations.
         * @return a sahred pointer to a new shader.
         */
        static std::shared_ptr<Shader> create(const std::string &vertexShader, const std::string &fragmentShader, std::vector<ShaderLocations> locations);

        virtual ~Shader() = default;

    private:
        std::string vertex, fragment;
        std::map<int,std::string> settedValues;
        static inline std::map<std::string,std::shared_ptr<Shader>> cachedShaders;
    protected:
        std::vector<ShaderTexture> textures;
    };
}
#endif //GLPROJECT_SHADER_H
