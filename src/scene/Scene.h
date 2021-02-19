//
// Created by William Lundqvist on 2020-11-12.
//

#ifndef GLPROJECT_SCENE_H
#define GLPROJECT_SCENE_H


#include <memory>
#include "../model/ModelInstance.h"
#include "../window/Window.h"
#include "../camera/Camera.h"
#include "Layer.h"

namespace GLProj {

    /**
     * class handling window and multiple layers.
     */
    class Scene {
    public:
        Scene(std::shared_ptr<Window> window);

        /**
         * updates the scene.
         */
        void update();

    protected:

        /**
         * add a new layer onto the scene
         * @tparam T type of the layer
         * @tparam Args arguments for the T layer constructor.
         * @param id id, each id most be unique
         * @param args arguments for the T layer constructor
         * @return returns a shared_ptr for T.
         */
        template<class T, class ... Args>
        std::shared_ptr<T> addLayer(int id, Args&& ... args) {
            std::shared_ptr<T> layer = std::make_shared<T>(std::forward<Args>(args)...);
            layers.push_back(layer);
            layer->init();
            layerMap[id] = layer;
            return layer;
        }

        /**
         * returns a layer with a given id
         * @tparam T the type of that layer with given id.
         * @param id id of the layer.
         * @return shared_ptr of the layer.
         */
        template<class T>
        std::shared_ptr<T> getLayer(int id) {
            return layerMap[id];
        }

    private:
        std::shared_ptr<Camera> createCamera();

        std::shared_ptr<Window> window;
        std::vector<std::shared_ptr<Layer>> layers;
        std::map<int,std::shared_ptr<Layer>> layerMap;

    };
}


#endif //GLPROJECT_SCENE_H
