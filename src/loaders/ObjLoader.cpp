//
// Created by William Lundqvist on 2020-10-30.
//

#include "ObjLoader.h"
#include "../tools/Tools.h"
#include "../Core.h"
#include <fstream>
#include <stdio.h>
#include <map>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <set>
/**
 * struct containing math for a vector4
 */
struct v4 {
    float x,y,z,w;
    int size;
    void operator=(const struct v4 v) {
        x = v.x;
        y = v.y;
        z = v.z;
        w = v.w;
        size = v.size;
    }
};

/**
 * struct containing math for a vector3
 */
struct v3 {
    float x,y,z;
    void operator=(const struct v3& v) {
        x = v.x;
        y = v.y;
        z = v.z;
    }
    void operator=(const struct v4 v) {
        x = v.x;
        y = v.y;
        z = v.z;
    }

    bool operator==(const struct v3& v) {
        return x == v.x && y == v.y && z == v.z;
    }

    v3 operator-(const struct v3& v) {
        v3 val = {x-v.x,y-v.y,z-v.z};
        return val;
    }

    v3 operator+(const struct v3& v) {
        v3 val = {x+v.x,y+v.y,z+v.z};
        return val;
    }

    v3 operator*(const struct v3& v) {
        v3 val = {x*v.x,y*v.y,z*v.z};
        return val;
    }

    v3 operator*(float scalar) {
        v3 val = {x*scalar,y*scalar,z*scalar};
        return val;
    }

    v3 normalize() {
        float len = length();
        v3 res = {
                x/len,
                y/len,
                z/len
        };
        return res;
    }

    v3 cross(v3& val) {
        glm::vec3 v = glm::vec3(val.x,val.y,val.z);
        glm::vec3 v2 = glm::vec3(x,y,z);
        glm::vec3 crs = glm::cross(v,v2);
        return {crs.x,crs.y,crs.z};
    }

    float length() {
        return std::sqrt(x*x+y*y+z*z);
    }
};

/**
 * struct containing math for a vector2
 */
struct v2 {
    float x,y;
    void operator=(const struct v2 v) {
        x = v.x;
        y = v.y;
    }
};

/**
 * struct containing information for a Face.
 */
struct Face {
    int i1,i2,i3;
    v3 p1,p2,p3;
    v3 normal;
};

/**
 * returns which dimenstion is the largets.
 * @param xDim x dimension
 * @param yDim y dimension
 * @param zDim z dimension
 * @return a vector3 where the non zero value is the biggest.
 */
static v3 getMaxDim(float xDim, float yDim, float zDim) {
    if(xDim > yDim) {
        if(xDim > zDim) {
            return {xDim,0,0};
        } else {
            return {0,0,zDim};
        }
    } else {
        if(yDim > zDim) {
            return {0,yDim,0};
        } else {
            return {0,0,zDim};
        }
    }
}

/**
 * struct used in a hashmap to hash a v3 into a integer key.
 */
struct KeyFuncs
{
    size_t operator()(const v3& k)const
    {
        return std::hash<int>()(k.x) ^ std::hash<int>()(k.y) ^ std::hash<int>()(k.z);
    }

    bool operator()(const v3& a, const v3& b)const
    {
        return a.x == b.x && a.y == b.y && a.z == b.z;
    }
};

std::shared_ptr<GLProj::OBJ> GLProj::ObjLoader::loadOBJ(const std::string &url, const ObjLoaderSettings& settings) {
    if(cache.find(url) != cache.end()) {
        return cache[url];
    }
    std::string line;
    std::ifstream file(url.c_str());
    FaceMap faceMap;

    std::unordered_map<std::string, int> indiciesMap;
    std::shared_ptr<OBJ> obj = std::make_shared<OBJ>();
    std::vector<Face> faces = std::vector<Face>();
    std::vector<std::string> values = std::vector<std::string>();
    std::vector<int> currentIndexList;
    std::vector<v3> faceVertices = std::vector<v3>();

    if(file.is_open()) {
        std::vector<std::string> split_line;
        std::vector<v4> vertices;
        std::vector<v3> colors;

        std::vector<v3> normals;
        std::vector<v2> texture;

        bool hasNormals = false, hasTextureTextureCoords = false;
        int index = 0,meshSize = 0;

        v3 zero = {0,0,0};
        v3 left=zero,right=zero,front=zero,back=zero,top=zero,bottom=zero;
        int vertexSize = 0;
        std::vector<v3> faceNormals = std::vector<v3>();

        while(std::getline(file,line)) {
            if(line.length() == 0) continue;
            if(line.at(0) == '#') continue;

            //texture
            if(line.at(0) == 'v' && line.at(1) == 't') {
                hasTextureTextureCoords = true;
                v2 v;
                sscanf(line.c_str(),"vt %f %f",&v.x,&v.y);
                texture.push_back(v);
            }

            //normal
            if(line.at(0) == 'v' && line.at(1) == 'n') {
                hasNormals = true;
                Tools::splitFast(line, ' ',split_line, 1);

                v3 v;
                sscanf(line.c_str(),"vn %f %f %f",&v.x,&v.y,&v.z);
                normals.push_back(v);
            }

            //position/color
            if(line.at(0) == 'v' && line.at(1) == ' ') {
                Tools::splitFast(line, ' ',split_line, 1);

                v4 v;
                v.size = 3;
                v.x = atof(split_line[0].c_str());
                v.y = atof(split_line[1].c_str());
                v.z = atof(split_line[2].c_str());

                //finds the edges.
                if(v.x < left.x)
                    left = v;
                if(v.x > right.x)
                    right = v;
                if(v.y < bottom.y)
                    bottom = v;
                if(v.y > top.y)
                    top = v;
                if(v.z < back.z)
                    back = v;
                if(v.z > front.z) {
                    front = v;
                }

                if(v.size > 3) {
                    v.w = atof(split_line[3].c_str());
                    v.size = 4;
                }

                vertices.push_back(v);
                if(v.size > 4) {
                    v3 col;
                    col.x = atof(split_line[4].c_str());
                    col.y = atof(split_line[5].c_str());
                    col.z = atof(split_line[6].c_str());
                    colors.push_back(col);
                }
            }

            //polygon
            if(line.at(0) == 'f') {

                if(obj->subMeshes.size() == 0) {
                    obj->addSubMesh("",0,0);
                }

                currentIndexList.clear();
                faceVertices.clear();
                Tools::splitFast(line, ' ', split_line,1);
                for(std::string& edge : split_line) {

                    //check if vertex already in array, use that index instead of creating new one.
                    if(indiciesMap.find(edge) != indiciesMap.end()) {
                        currentIndexList.push_back(indiciesMap[edge]);
                        continue;
                    }
                    indiciesMap[edge] = index;
                    currentIndexList.push_back(index);
                    index++;
                    meshSize++;
                    Tools::splitFast(edge,'/',values,0,true);
                    int dataPerVertex = 0;
                    int attributeIndex = 0;
                    for(std::string& val : values) {
                        if(val.length() > 0) {
                            int valIndex = atoi(val.c_str())-1;
                            //position
                            switch(attributeIndex) {
                                case 0: {
                                    v4 v = vertices[valIndex];
                                    //faceVertices.push_back({v.x,v.y,v.z});
                                    if(v.size > 3) {
                                        obj->addPosition(v.x,v.y,v.z,v.w);
                                        vertexSize = 4;
                                    } else {
                                        obj->addPosition(v.x,v.y,v.z);
                                        vertexSize = 3;
                                    }

                                    if(colors.size() > valIndex) {
                                        v3 col = colors[valIndex];
                                        obj->addColor(col.x,col.y,col.z);
                                        dataPerVertex += 3;
                                    }
                                }break;

                                //texture
                                case 1: {
                                    v2 uv = texture[valIndex];
                                    dataPerVertex += 2;
                                    obj->addTextureCoord(uv.x,uv.y);
                                }break;

                                //normals
                                case 2: {
                                    v3 v = normals[valIndex];
                                    dataPerVertex += 3;
                                    obj->addNormal(v.x,v.y,v.z);
                                }break;
                            }
                        }

                        attributeIndex ++;
                    }
                }
                auto& poses = obj->positions();
                //triangluate
                for(int i = 2; i < currentIndexList.size(); i++) {
                    std::vector<int>& indexArr = obj->subMeshes.rbegin()->indicies;
                    int start = obj->subMeshes.rbegin()->vertexIndex;
                    int vert1 = start+currentIndexList[0];
                    int vert2 = start+currentIndexList[i-1];
                    int vert3 = start+currentIndexList[i];
                    indexArr.push_back(vert1-start);
                    indexArr.push_back(vert2-start);
                    indexArr.push_back(vert3-start);

                    if(!hasNormals && settings.generateNormals) {

                        //gets the correct position corresponding index;
                        v3 p1 = {poses[vert1*3],poses[vert1*3+1],poses[vert1*3+2]};
                        v3 p2 = {poses[vert2*3],poses[vert2*3+1],poses[vert2*3+2]};
                        v3 p3 = {poses[vert3*3],poses[vert3*3+1],poses[vert3*3+2]};

                        v3 QP = p1 - p2;
                        v3 QS = p3 - p2;
                        v3 n = QP.cross(QS).normalize();
                        Face f = {vert1-start,vert2-start,vert3-start,p1,p2,p3,n};
                        faces.push_back(f);

                        //adds into face map to speed up vertex normal generation.
                        faceMap[p1].push_back(f);
                        faceMap[p2].push_back(f);
                        faceMap[p3].push_back(f);
                    }
                }
            }

        }
        file.close();

        if(obj->subMeshes.size() > 0) {
            obj->subMeshes.rbegin()->size = index;
        }

        if(settings.normalized) {
            normalize(obj, left, right, front, back, top, bottom, vertexSize);
        }

        if(settings.generateNormals && !hasNormals) {
            calculateVertexNormals(faceMap, obj, faces);
        }

    } else {
        fprintf(stderr,"Could not load Obj file: %s\n",url.c_str());
        return nullptr;
    }

    cache[url] = obj;
    return obj;
}

void GLProj::ObjLoader::normalize(std::shared_ptr<GLProj::OBJ> &obj, const v3 &left, const v3 &right, const v3 &front,
                          const v3 &back, const v3 &top, const v3 &bottom,
                          int vertexSize) {//rescale all vector to fit inside a 1x1x1 cube.

    v3 zero = {0,0,0};
    float xdim = right.x - left.x;
    float ydim = top.y - bottom.y;
    float zdim = front.z - back.z;

    //if it is already inside a 1x1x1, dont rescale.
    if(!(xdim <= 1 && ydim <= 1 && zdim <= 1)) {

        v3 center = {
                left.x + xdim * 0.5f,
                bottom.y + ydim * 0.5f,
                back.z + zdim * 0.5f
        };

        //returns the largets dimension of xdim, ydim or zdim.
        //if xdim is largets, return will be v3{xdim,0,0}
        v3 maxDim = getMaxDim(xdim, ydim, zdim);

        float scaleFactor;

        //calculate how much to scale down the model to fit inside a 1x1x1 volume.
        if (maxDim.x != 0) {
            float lengthX = right.x - center.x;
            scaleFactor = (1.0f - 0.5f / lengthX) ;
        } else if (maxDim.y != 0) {
            float lengthY = top.y - center.y;
            scaleFactor = (1.0f - 0.5f / lengthY) ;
        } else {
            float lengthZ = front.z - center.z;
            scaleFactor = (1.0f - (0.5f / lengthZ));
        }

        //update all the vertices so it scales down correctly.
        std::vector<float>& positions = obj->positions();
        for (int i = 0; i < positions.size(); i += vertexSize) {
            v3 val = {positions[i], positions[i + 1], positions[i + 2]};

            //moves center to zero.
            val = val + (zero - center);
            //shrink position towards center(which is zero) with a factor of scaleFactor.
            val = val + ((zero - val) * scaleFactor);

            positions[i] = val.x;
            positions[i + 1] = val.y;
            positions[i + 2] = val.z;
        }

    }
}

static v3 calculateVertexNormal(FaceMap& map,v3 vertex) {
    v3 vertexNormal = {0,0,0};
    auto& faces = map[vertex];
    for(Face& face : faces) {
        vertexNormal = vertexNormal + face.normal;
    }
    return vertexNormal.normalize();
}

void GLProj::ObjLoader::calculateVertexNormals(FaceMap& map,std::shared_ptr<OBJ>& obj, std::vector<Face>& faces) {
    auto& poses = obj->positions();
    std::unordered_set<int> calculatedVertices;
    for(auto& face : faces) {

        if(calculatedVertices.find(face.i1) == calculatedVertices.end()) {
            calculatedVertices.insert(face.i1);
            v3 n = calculateVertexNormal(map, face.p1);
            obj->addNormal(n.x,n.y,n.z);

        }

        if(calculatedVertices.find(face.i2) == calculatedVertices.end()) {
            calculatedVertices.insert(face.i2);
            v3 n = calculateVertexNormal(map, face.p2);
            obj->addNormal(n.x,n.y,n.z);
        }

        if(calculatedVertices.find(face.i3) == calculatedVertices.end()) {
            calculatedVertices.insert(face.i3);
            v3 n = calculateVertexNormal(map, face.p3);
            obj->addNormal(n.x,n.y,n.z);
        }
    }
}