//
// Created by William Lundqvist on 2020-10-30.
//

#ifndef PROJEKT_OBJ_H
#define PROJEKT_OBJ_H

#include <vector>
#include <memory>
#include <map>
#include <string>
class ObjLoader;
namespace GLProj {

    /**
     * struct for a mesh the is building blocks for a OBJ file.
     */
    struct SubMesh {
        std::string material;
        int vertexIndex;
        int size;
        std::vector<int> indicies = std::vector<int>();
    };

    /**
     * class file prepresenting a OBJ file
     */
    class OBJ {
    public:
        OBJ() {}

        /**
         * adds a new vertex to the obj file.
         */
        void addPosition(float x,float y, float z,float w);

        void addPosition(float x,float y, float z);

        /**
         * adds a submesh to the obj file.
         */
        void addSubMesh(std::string material, int index, int size);

        /**
         * adds a new color to the obj file.
         */
        void addColor(float red, float green, float blue);

        /**
         * adds a new normal to the obj file.
         */
        void addNormal(float dx,float dy, float dz);

        /**
         * adds a new texture coordinate to the obj file.
         */
        void addTextureCoord(float u,float v);

        std::vector<float>& positions() {
            return pos;
        }

        const std::vector<float>& colors() {
            return cols;
        }

        const std::vector<float>& normals() {
            return norms;
        }

        std::vector<float>& texturecoords() {
            return t_coords;
        }

        const std::vector<SubMesh>& meshes() {
            return subMeshes;
        }

        std::vector<SubMesh> subMeshes;
    private:
        std::vector<float> pos;
        std::vector<float> cols;
        std::vector<float> norms;
        std::vector<float> t_coords;
        int dataPerVertex;
        friend class ObjLoader;
    };
}

#endif //PROJEKT_OBJ_H
