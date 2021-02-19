//
// Created by William Lundqvist on 2020-11-06.
//

#ifndef GLPROJECT_SHADEROPENGL_H
#define GLPROJECT_SHADEROPENGL_H


#include <glad/glad.h>
#include "../../graphics/Shader.h"
namespace GLProj {

    /**
     * Shader implementation for opengl. all method comments are commented in Shader .h file.
     */
    class ShaderOpenGL : public Shader {
    public:
        ShaderOpenGL(const std::string& vertex, const std::string& fragment,std::vector<ShaderLocations> locations);


        void bind() override;

        void unbind() override;

        void setUniform(const std::string &name, glm::mat4 value) override;

        void setUniform(const std::string &name, glm::vec3 value) override;

        void setUniform(const std::string &name, glm::vec2 value) override;

        void setUniform(const std::string &name, glm::vec4 value) override;

        void setUniform(const std::string &name, float value) override;

        void setUniformi(const std::string &name, int value) override;


        void destroy() override;

        ~ShaderOpenGL() override;

    private:
        int uniformLocation(const std::string& name);
        int parseShader(GLuint id,const std::string& file);
        GLuint program;
        void bindAttribute(const std::string &name, int index);
        std::map<std::string, int> uniformLocations;

    };
}


#endif //GLPROJECT_SHADEROPENGL_H
