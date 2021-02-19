//
// Created by William Lundqvist on 2020-11-05.
//

#include <glad/glad.h>
#include "Model.h"
#include "../Core.h"
#include "ModelInstance.h"

std::shared_ptr<GLProj::Model> GLProj::Model::create(std::shared_ptr<OBJ> obj) {
    std::shared_ptr<Model> model = std::make_shared<Model>(obj->subMeshes,obj->positions(),obj->colors(),obj->normals(),obj->texturecoords());
    //model->setMaterials(obj);
    return model;
}

std::shared_ptr<GLProj::Model> GLProj::Model::create(std::vector<float> positions, std::vector<int> indicies) {
    std::shared_ptr<Model> mesh = std::make_shared<Model>(std::vector<SubMesh>(),positions,std::vector<float>(),std::vector<float>(),std::vector<float>());
    return mesh;
}

std::shared_ptr<GLProj::ModelInstance> GLProj::Model::createInstance() {
    std::shared_ptr<ModelInstance> val = std::make_shared<ModelInstance>();
    val->setModel(shared_from_this());

    return val;
}

GLProj::Model::Model(const std::vector<SubMesh> &subMeshes, const std::vector<float> &positions,
                     const std::vector<float> &color, const std::vector<float> &normals,
                     const std::vector<float> &texturecoords)  {
    if(texturecoords.size() > 0)
        textureCoords = true;

    for(SubMesh mesh : subMeshes) {
        Mesh glMesh;

        glGenVertexArrays(1, &glMesh.vao);
        glBindVertexArray(glMesh.vao);
        glGenBuffers(1, &glMesh.ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glMesh.ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indicies.size() * sizeof(GLint), &mesh.indicies.data()[0],
                     GL_STATIC_DRAW);

        GLuint buff;
        glGenBuffers(1, &buff);
        glBindBuffer(GL_ARRAY_BUFFER, buff);

        int start = mesh.vertexIndex;
        int size = mesh.size;

        glBufferData(GL_ARRAY_BUFFER, (size*3*3+size*2)*sizeof(GLfloat), NULL, GL_STATIC_DRAW);
        fillBuffer(&glMesh,&glMesh.positionBuffer, positions,0, start*3, size*3,  3,0);

        //if (color.size() > 0)
            //fillBuffer(&glMesh,&glMesh.colorBuffer, color,size*3, start*3, size*3, 3,3);

        if (normals.size() > 0) {
            fillBuffer(&glMesh, &glMesh.normalBuffer, normals,size*3, start * 3, size * 3,3,1);
        }
        if (texturecoords.size() > 0) {
            fillBuffer(&glMesh, &glMesh.uvBuffer, texturecoords,glMesh.activeIndex*size*3+size*3, start * 2, size * 2, 2,2);
        }

        glMesh.indexSize = mesh.indicies.size();

        glMesh.materialName = mesh.material;
        this->subMeshes.push_back(glMesh);
    }
    glBindVertexArray(0);
}

void GLProj::Model::fillBuffer(Mesh* mesh, GLuint * id, std::vector<float> data,int offset, int start, int size, int vertexSize,int index) {
    glBufferSubData(GL_ARRAY_BUFFER, offset*sizeof(GLfloat), size*sizeof(GLfloat), &data.data()[start]);
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index,vertexSize,GL_FLOAT,GL_FALSE,0,BUFFER_OFFSET(offset*sizeof(GLfloat)));

    //mesh->boundAttributeLocations[name] = mesh->activeIndex;
    mesh->activeIndex ++;
}