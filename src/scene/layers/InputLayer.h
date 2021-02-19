//
// Created by William Lundqvist on 2020-12-05.
//

#ifndef GLPROJECT_INPUTLAYER_H
#define GLPROJECT_INPUTLAYER_H


#include <memory>
#include "../Layer.h"
#include "../../camera/Camera.h"
#include "../../model/ModelInstance.h"

namespace GLProj {

    /**
     * a layer handling input events from a given window and applies it to a given instance and camera.
     */
    class InputLayer : public Layer {
    public:
        InputLayer(std::shared_ptr<Camera> camera, std::shared_ptr<ModelInstance> instance, std::shared_ptr<Window> window);

        void init() override;

        void update() override;

        ~InputLayer() override;
    private:
        std::shared_ptr<Camera> camera;
        std::shared_ptr<ModelInstance> instance;
        std::shared_ptr<Window> window;
    };
}


#endif //GLPROJECT_INPUTLAYER_H
