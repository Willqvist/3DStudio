//
// Created by William Lundqvist on 2020-11-06.
//

#ifndef GLPROJECT_RENDERER_H
#define GLPROJECT_RENDERER_H


#include <glad/glad.h>
#include <memory>
#include "../model/ModelInstance.h"
#include "../camera/Camera.h"
#include "Light.h"
#include "../scene/layers/RenderingLayer.h"
#include "FBO.h"

namespace GLProj {

    /**
     * static class handling sending/rendering data to the graphics card.
     */
    class Renderer {
    public:
        /**
         * binds a camera to be used when rendering
         * @param camera the camera to render
         */
        static void bindCamera(std::shared_ptr<Camera>& camera);

        /**
         * inits the shader wiuth a given window.
         */
        static void init(std::shared_ptr<Window>&);

        /**
         * renders a model instance with a list of lights affecting it.
         * @param drawMode drawMode to use. usually TRIANGLES
         * @param instance the instance to render
         * @param lights the lights to affect the instance.
         */
        static void render(GLuint drawMode, std::shared_ptr<ModelInstance> instance, std::vector<LightInstance> lights);

        /**
         * clear the rendering and cached values.
         */
        static void clear();

        /**
         * renders a shadow map for a the given lights.
         * @param window window to to render to.
         * @param instances instances to render
         * @param lights lights to calculate shadow maps for.
         */
        static void renderShadowMaps(std::shared_ptr<Window>& window, std::vector<RenderingInstance>& instances, std::vector<LightInstance> lights);

    private:

        /**
         * binds a given light and binds all uniforms.
         * @param light the light to bind.
         * @param index what index the light is at.
         * @param shader what shader ti set the uniforms in.
         */
        static void bindLight(std::shared_ptr<Light> light,int index, std::shared_ptr<Shader> shader);
        static inline std::shared_ptr<Camera> boundCamera;
        static inline std::shared_ptr<Camera> shadowMapCamera;
        static inline glm::mat4 projection;
        static inline std::shared_ptr<Shader> shadowShader;
        static inline std::shared_ptr<FBO> shadowMapFbo;
        static inline bool hasFoundDirectionalLight = false;
    };
}


#endif //GLPROJECT_RENDERER_H
