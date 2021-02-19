//
// Created by William Lundqvist on 2020-10-30.
//

#include "OBJ.h"

void GLProj::OBJ::addPosition(float x, float y, float z, float w) {
    pos.push_back(x);
    pos.push_back(y);
    pos.push_back(z);
    pos.push_back(w);
}

void GLProj::OBJ::addPosition(float x, float y, float z) {
    pos.push_back(x);
    pos.push_back(y);
    pos.push_back(z);
}

void GLProj::OBJ::addColor(float red, float green, float blue) {
    cols.push_back(red);
    cols.push_back(green);
    cols.push_back(blue);
}

void GLProj::OBJ::addNormal(float dx, float dy, float dz) {
    norms.push_back(dx);
    norms.push_back(dy);
    norms.push_back(dz);
}

void GLProj::OBJ::addTextureCoord(float u, float v) {
    t_coords.push_back(u);
    t_coords.push_back(v);
}

void GLProj::OBJ::addSubMesh(std::string material, int index, int size) {
    SubMesh mesh = {material,index,size};
    subMeshes.push_back(mesh);
}
