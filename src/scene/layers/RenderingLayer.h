//
// Created by William Lundqvist on 2020-12-05.
//

#ifndef GLPROJECT_RENDERINGLAYER_H
#define GLPROJECT_RENDERINGLAYER_H


#include <memory>
#include "../Layer.h"
#include "../../model/ModelInstance.h"
#include "../../graphics/Light.h"
#include "../../window/Window.h"

namespace GLProj {

    /**
     * struct of a ModelInstance with a name
     */
    struct RenderingInstance {
        std::string name;
        std::shared_ptr<ModelInstance> instance;
    };

    /**
     * struct of a Light with a name
     */
    struct LightInstance {
        std::string name;
        std::shared_ptr<Light> instance;
    };

    /**
     * a layer handling rendering multiple ModelInstances and lights.
     */
    class RenderingLayer : public Layer {
    public:

        RenderingLayer(std::shared_ptr<Window>& window);

        void init() override;

        /**
         * updates and renders all instances and lights.
         */
        void update() override;

        /**
         * adds a new instance to the layer.
         * @param instance the instance to add
         * @param name name of the instance
         */
        void addInstance(std::shared_ptr<ModelInstance> instance,std::string name = "");

        /**
         * adds a new light to the layer
         * @param instance instance of the light
         * @param name name of the light.
         */
        void addLight(std::shared_ptr<Light> instance,std::string name = "");

        /**
         * sets the rendering mode for all instances added.
         * @param mode
         */
        void setMode(GLuint mode);

        std::vector<RenderingInstance> getInstances() {
            return instances;
        }

        std::vector<LightInstance> getLights() {
            return lights;
        }

        ~RenderingLayer() override;
    private:
        std::vector<RenderingInstance> instances;
        std::vector<LightInstance> lights;
        std::shared_ptr<Window> window;
        std::map<std::string, RenderingInstance> instanceMap;
        std::map<std::string, LightInstance> lightsMap;
        GLuint mode = GL_TRIANGLES;
    };
}


#endif //GLPROJECT_RENDERINGLAYER_H
