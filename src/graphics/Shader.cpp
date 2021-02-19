//
// Created by William Lundqvist on 2020-11-06.
//
#include "Shader.h"
#include "../platform/opengl/ShaderOpenGL.h"

GLProj::Shader::Shader(const std::string &vertexShader, const std::string &fragmentShader,std::vector<ShaderLocations> locations) : vertex(vertexShader),fragment(fragmentShader) {}

std::shared_ptr<GLProj::Shader> GLProj::Shader::create(const std::string &vertexShader, const std::string &fragmentShader,std::vector<ShaderLocations> locations) {
    std::string key = vertexShader + ":" + fragmentShader;
    if(cachedShaders.find(key) != cachedShaders.end())
        return cachedShaders[key];
    std::shared_ptr<Shader> shader = std::make_shared<ShaderOpenGL>(vertexShader,fragmentShader,locations);
    cachedShaders[key] = shader;
    return shader;
}

void GLProj::Shader::set(int id, const std::string &name) {
    settedValues[id] = name;
}

const std::string &GLProj::Shader::get(int id) {
    return settedValues[id];
}