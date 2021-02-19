//
// Created by William Lundqvist on 2020-11-12.
//

#include "Mesh.h"

void GLProj::Mesh::bind() {
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);
}

void GLProj::Mesh::unbind() {
    glBindVertexArray(0);
}

