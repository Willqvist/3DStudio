//
// Created by William Lundqvist on 2020-11-19.
//

#ifndef GLPROJECT_LAYER_H
#define GLPROJECT_LAYER_H

/**
 * a layer that can be added upon to a scene.
 */
class Layer {
public:

    virtual void init() = 0;
    virtual void update() = 0;
    virtual ~Layer() {}
};
#endif //GLPROJECT_LAYER_H
