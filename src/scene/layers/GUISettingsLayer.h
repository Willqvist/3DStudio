//
// Created by William Lundqvist on 2020-12-05.
//

#ifndef GLPROJECT_GUISETTINGSLAYER_H
#define GLPROJECT_GUISETTINGSLAYER_H


#include <memory>
#include "../Layer.h"
#include "../../camera/Camera.h"
#include "RenderingLayer.h"

namespace GLProj {

    /**
     * layer handling rendering GUI using ImGui onto a given window.
     * It changes settings for a camera and rendering mode.
     */
    class GUISettingsLayer : public Layer {
    public:

        GUISettingsLayer(std::shared_ptr<Window> window,std::shared_ptr<Camera> camera, std::shared_ptr<RenderingLayer> renderingLayer);

        void init() override;

        void update() override;

        ~GUISettingsLayer() override;
    private:
        bool activePers = true;
        float fov = 60, zNear = 0.01f,zFar = 1000.0f;
        float farPar = 1000,topPar=1.0f, obliqueScale = 0, obliqueAngle = 45;
        std::shared_ptr<Window> window;
        std::shared_ptr<Camera> camera;
        std::shared_ptr<RenderingLayer> renderingLayer;
        bool open = true;
        int menuItem = 0;

        std::vector<GLProj::ShaderLocations> shaderLocations = {
                {"vPosition",0},
                {"vNormal",1},
                {"vTextureCoords",2},
                {"vColor",3},
        };

        void renderCameraSettings();
        void renderRendererSettings();
    };
}

#endif //GLPROJECT_GUISETTINGSLAYER_H
