//
// Created by William Lundqvist on 2020-11-19.
//

#ifndef GLPROJECT_MATERIAL_H
#define GLPROJECT_MATERIAL_H


#include <memory>
#include "Texture.h"
#include "Shader.h"
#include <variant>
namespace GLProj {

    /**
     * a texture for the material wtih a name bound to it.
     */
    struct MaterialTexture {
        std::string name;
        std::shared_ptr<Texture> texture;
    };


    typedef std::variant<int,float,bool,glm::vec3,glm::vec4,glm::vec2,glm::mat4> MaterialVariable;

    /**
     * visitor struct used to seperate values in a variant and set appropiate values in a given shader.
     */
    struct VisitShader
    {
    public:
        std::shared_ptr<Shader> shader;
        std::string name;
        void operator () (int& val ) {
            shader->setUniformi(name, val);
        }
        void operator () (float& val ) {
            shader->setUniform(name, val);
        }
        void operator () (bool& val ) {
            shader->setUniform(name, val);
        }
        void operator () (glm::vec3& val ) {
            shader->setUniform(name, val);
        }
        void operator () (glm::vec4& val ) {
            shader->setUniform(name, val);
        }
        void operator () (glm::vec2& val ) {
            shader->setUniform(name, val);
        }
        void operator () (glm::mat4& val ) {
            shader->setUniform(name, val);
        }
    };

    /**
     * material class built using a shader and stores values.
     */
    class Material {
    public:
        Material(std::shared_ptr<Shader> shader);

        /**
         * sets a shader for the material
         * @param shader the shader to set in this material.
         */
        void setShader(std::shared_ptr<Shader> shader);

        /**
         * @return the set shader of this material
         */
        std::shared_ptr<Shader> getShader();

        /**
         * sets a value for a uniform in a shader,
         * @tparam T the type of the set value
         * @param name the uniform name
         * @param value the value to set.
         */
        template<class T>
        void set(const std::string& name, const T& value) {
            this->variables[name] = value;
        }

        /**
         * @return all set values that is set via set()
         */
        std::map<std::string,MaterialVariable> getValues() {
            return variables;
        }

        /**
         * sets a texture to the shader with a name
         * @param name
         * @param texture
         */
        void set(const std::string& name, std::shared_ptr<Texture> texture);

        std::vector<MaterialTexture> getTextures() {
            return textures;
        }

        void bind();

        void unbind();
    private:
        std::shared_ptr<Shader> shader;
        std::vector<MaterialTexture> textures;
        std::map<std::string,MaterialVariable> variables;
        VisitShader visitor;
    };
}


#endif //GLPROJECT_MATERIAL_H
