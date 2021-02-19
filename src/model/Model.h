//
// Created by William Lundqvist on 2020-11-05.
//

#ifndef GLPROJECT_MODEL_H
#define GLPROJECT_MODEL_H


#include <memory>
#include "../loaders/OBJ.h"
#include "glad/glad.h"
#include "Mesh.h"


namespace GLProj {
    class ModelInstance;

    /**
     * Model that has been loaded into the graphics card.
     */
    class Model : public std::enable_shared_from_this<Model> {
    public:
        Model(const std::vector<SubMesh>& subMeshes,const std::vector<float>& positions,const std::vector<float>& color, const std::vector<float>& normals, const std::vector<float>& texturecoords);

        /**
         * creates a instance from the model.
         * @return shared_ptr of a ModelInstance.
         */
        std::shared_ptr<ModelInstance> createInstance();

        /**
         * returns a list of all the meshes this model is built upon.
         * @return vector of meshes.
         */
        std::vector<Mesh>& meshes() {
            return subMeshes;
        }

        /**
         * returns true if the loaded model has texture coords.
         * @return true if it has texture coordinates.
         */
        bool hasTextureCoords() {
            return textureCoords;
        }

        /**
         * creates a Model from a OBJ file
         * @param obj the obj file.
         * @return Model
         */
        static std::shared_ptr<Model> create(std::shared_ptr<OBJ> obj);

        /**
         * creates a model from a lsit of positions and indicies.
         * @param positions the positions
         * @param indicies the indicies
         * @return a new model
         */
        static std::shared_ptr<Model> create(std::vector<float> positions, std::vector<int> indicies);

    private:
        void fillBuffer(Mesh* mesh,GLuint * id, std::vector<float> data,int offset, int start, int size, int vertexSize,int index);

        bool textureCoords = false;
        std::vector<Mesh> subMeshes;
        friend class ModelInstance;
    };
}

#endif //GLPROJECT_MODEL_H
