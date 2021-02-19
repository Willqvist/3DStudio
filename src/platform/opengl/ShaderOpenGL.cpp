//
// Created by William Lundqvist on 2020-11-06.
//

#include "ShaderOpenGL.h"
#include "../../tools/Tools.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include "../../Core.h"

GLProj::ShaderOpenGL::ShaderOpenGL(const std::string &vertex,const  std::string &fragment,std::vector<ShaderLocations> locations): Shader(vertex,fragment,locations) {
    program = glCreateProgram();

    for(auto& loc : locations) {
        bindAttribute(loc.name,loc.location);
    }

    GLuint vId = parseShader(GL_VERTEX_SHADER, vertex);
    GLuint fId = parseShader(GL_FRAGMENT_SHADER, fragment);
    glLinkProgram(program);

    GLint linkStatus;
    glGetProgramiv(program, GL_LINK_STATUS,&linkStatus);
    if(!linkStatus) {
        DEBUG("Failed to link shader program");
        char msg[1024];
        glGetProgramInfoLog(program,1024,nullptr,&(msg[0]));
        DEBUG(msg);
    }
}

void GLProj::ShaderOpenGL::bind() {
    glUseProgram(program);
    for(ShaderTexture& tex : textures) {
        setUniformi(tex.name,(int)tex.slot);
        tex.texture->bind(tex.slot);
    }
}

void GLProj::ShaderOpenGL::unbind() {
    glUseProgram(0);
}

void GLProj::ShaderOpenGL::setUniform(const std::string &name, glm::mat4 value) {
    glUniformMatrix4fv(uniformLocation(name),1,GL_FALSE,glm::value_ptr(value));
}

void GLProj::ShaderOpenGL::setUniform(const std::string &name, glm::vec3 value) {
    glUniform3fv(uniformLocation(name),1,glm::value_ptr(value));
}

void GLProj::ShaderOpenGL::setUniform(const std::string &name, glm::vec2 value) {
    glUniform2fv(uniformLocation(name),1,glm::value_ptr(value));
}

void GLProj::ShaderOpenGL::setUniform(const std::string &name, glm::vec4 value) {
    glUniform4fv(uniformLocation(name),1,glm::value_ptr(value));
}

void GLProj::ShaderOpenGL::setUniform(const std::string &name, float value) {
    glUniform1f(uniformLocation(name),value);
}

void GLProj::ShaderOpenGL::setUniformi(const std::string &name, int value) {
    glUniform1i(uniformLocation(name),value);
}

void GLProj::ShaderOpenGL::destroy() {
    glDeleteProgram(program);
}

GLProj::ShaderOpenGL::~ShaderOpenGL() {
    destroy();
}

int GLProj::ShaderOpenGL::uniformLocation(const std::string &name) {
    if(uniformLocations.find(name) != uniformLocations.end()) {
        return uniformLocations[name];
    }
    int loc = glGetUniformLocation(program,name.c_str());
    uniformLocations[name] = loc;
    return loc;
}

int GLProj::ShaderOpenGL::parseShader(GLuint id,const std::string &file) {
    GLuint shader = glCreateShader(id);
    std::string src = Tools::parseShader(file);
    const char* contentsChars = src.c_str();
    glShaderSource(shader,1,&contentsChars,NULL);
    glCompileShader(shader);
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if(!status) {
        DEBUG("Failed to compile '" << file << "'");
        char msg[1024];
        glGetShaderInfoLog(shader,1024,nullptr,&(msg[0]));
        DEBUG(msg);
    }
    glAttachShader(program, shader);
    return shader;
}

void GLProj::ShaderOpenGL::bindAttribute(const std::string &name, int index) {
    glBindAttribLocation(program,index,name.c_str());
}
