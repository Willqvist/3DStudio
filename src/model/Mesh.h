//
// Created by William Lundqvist on 2020-11-12.
//

#ifndef GLPROJECT_MESH_H
#define GLPROJECT_MESH_H


#include <glad/glad.h>
#include <map>
#include <string>
#include <memory>
#include "../graphics/Shader.h"
#include "../graphics/Material.h"

namespace GLProj {

    /**
     * A mesh is a object built with vertices, normals, color, uv
     */
    class Mesh {
    public:

        /**
         * binds the mesh to be rendererd.
         */
        void bind();

        /**
         * unbinds this mesh.
         */
        void unbind();

        /**
         * attributes for vbo and vaos
         */
        GLuint vao, ibo, positionBuffer, normalBuffer, uvBuffer;
        int indexSize, activeIndex = 0;
        std::shared_ptr<Material> material;
        std::string materialName;
    };
}

#endif //GLPROJECT_MESH_H
