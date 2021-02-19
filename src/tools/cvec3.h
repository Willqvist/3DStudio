//
// Created by William Lundqvist on 2020-11-17.
//

#ifndef GLPROJECT_CVEC3_H
#define GLPROJECT_CVEC3_H

#include <glm/vec3.hpp>

namespace glm {

    /**
     * class containing constat vector3 values.
     */
    class cvec3 {
    public:
        static inline vec3 right = vec3(1, 0, 0);
        static inline vec3 up = vec3(0, 1, 0);
        static inline vec3 forward = vec3(0, 0, 1);
        static inline vec3 one = vec3(1, 1, 1);
    };
}
#endif //GLPROJECT_CVEC3_H
