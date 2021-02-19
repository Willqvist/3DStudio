//
// Created by William Lundqvist on 2020-12-06.
//

#ifndef GLPROJECT_LIGHT_H
#define GLPROJECT_LIGHT_H

#include <glm/vec3.hpp>
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace GLProj {

    /**
     * type of light
     */
    enum LightType {
        POINT, DIRECTIONAL
    };

    /**
     * a class containing data for a light,.
     */
    class Light {
    public:
        glm::vec3 position = glm::vec3(2,2,2);
        LightType type = POINT;
        glm::vec3 direction = glm::vec3(0,-1,0);
        glm::vec3 color = glm::vec3(0,0,1);
        float radius = 1;

        //shadowmap texture, it is non -1 if castShadows = true.
        GLuint depthTexture = -1;
        bool castShadows = false;

        //viewmatrix for a light to be used when calculating shadowmap.
        glm::mat4 spaceMatrix;
        float shadowIntensity = 0.8f;
    };
}


#endif //GLPROJECT_LIGHT_H
