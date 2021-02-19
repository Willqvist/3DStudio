//
// Created by William Lundqvist on 2020-11-05.
//

#ifndef GLPROJECT_MODELINSTANCE_H
#define GLPROJECT_MODELINSTANCE_H

#include <glm/mat4x4.hpp>
#include "../graphics/Shader.h"
#include "Mesh.h"
#include "../Moveable.h"
#include "../graphics/Material.h"
#include "Model.h"

namespace GLProj {
    class Renderer;

    /**
     * Instance of a model that can be scaled,moved,rotated and shaded.
     */
    class ModelInstance: public Moveable {
    public:

        ModelInstance() {}
        /**
         * sets the material for the model.
         * @param material  the material
         * @param index what index mesh to apply the material to.
         * @param copy true if the material should be copied, true by default
         */
        void setMaterial(std::shared_ptr<Material> material,int index, bool copy=true);

        /**
         * sets the material for all meshes in the model.
         * @param material  the material
         * @param copy true if the material should be copied, true by default
         */
        void setMaterial(std::shared_ptr<Material> material, bool copy=true);

        /**
         * sets the model for this instance.
         * @param model  the model this instance if a instance of.
         */
        void setModel(std::shared_ptr<Model> model);

        /**
         * @return the parent model
         */
        std::shared_ptr<Model> getModel();

        /**
         * material set through setMaterial() without the index.
         * @return the material
         */
        std::shared_ptr<Material> getMaterial();

        /**
         * returns the mesh at a given index.
         * @param index index start from 0.
         * @return a given mesh.
         */
        Mesh& mesh(int index);

        static const int ModelMatrixPosition = 0;
        static const int ViewProjectionPosition = 1;
    private:
        std::shared_ptr<Model> parent = nullptr;
        std::map<int,std::shared_ptr<Material>> materials;
        std::shared_ptr<Material> material;
        friend class Model;
        friend class Renderer;

    };
}


#endif //GLPROJECT_MODELINSTANCE_H
