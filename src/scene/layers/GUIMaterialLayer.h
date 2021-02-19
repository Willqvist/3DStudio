//
// Created by William Lundqvist on 2020-12-05.
//

#ifndef GLPROJECT_GUIMATERIALLAYER_H
#define GLPROJECT_GUIMATERIALLAYER_H


#include <memory>
#include "../Layer.h"
#include "RenderingLayer.h"
#include "../../window/Window.h"
#include "../../camera/Camera.h"
#include <imgui/imgui.h>

namespace GLProj {
    /**
     * visitor struct used to visit a material and render its properties into a gui.
     */
    struct VisitMaterialValue
    {
    public:
        std::string name;
        std::shared_ptr<Material> material;

        /**
         * renders any size of a vector.
         * @param val the vector to render
         */
        template<int size>
        bool renderVector(glm::vec<size, float, glm::defaultp>& data) {
            bool changed = false;
            for(int i = 0; i < size; i++) {
                ImGui::PushID(i);
                ImGui::PushItemWidth(52.f);
                changed |= ImGui::InputFloat(i == size - 1 ? name.c_str() : "##vec3x",&data[i],0,0,3,0);
                ImGui::PopID();
                ImGui::SameLine();
            }
            ImGui::NewLine();
            return changed;
        }

        /**
         * renders an int to a gui
         */
        void operator () (int& val ) {
            ImGui::PushItemWidth(64.f);
            if(ImGui::InputInt(name.c_str(),&val)) {
                material->set(name,val);
            }
        }

        /**
         * renders an float to a gui
         */
        void operator () (float& val ) {
            ImGui::PushItemWidth(64.f);
            if(ImGui::InputFloat(name.c_str(),&val)) {
                material->set(name,val);
            }
        }

        /**
         * renders an bool to a gui
         */
        void operator () (bool& val ) {
            ImGui::PushItemWidth(64.f);
            if(ImGui::RadioButton(name.c_str(),val)) {
                material->set(name,!val);
            }
        }

        /**
         * renders an vec3 to a gui
         */
        void operator () (glm::vec3& val ) {
            bool changed = renderVector(val);
            if(changed){
                material->set(name,val);
            }
        }

        /**
         * renders an vec4 to a gui
         */
        void operator () (glm::vec4& val ) {
            bool changed = renderVector(val);
            if(changed){
                material->set(name,val);
            }
        }

        /**
         * renders an vec2 to a gui
         */
        void operator () (glm::vec2& val ) {
            bool changed = renderVector(val);
            if(changed){
                material->set(name,val);
            }
        }

        void operator () (glm::mat4& val ) {
        }
    };

    /**
     * layer handling rendering material and lightr gui settings onto a window.
     */
    class GUIMaterialLayer : public Layer {
    public:

        GUIMaterialLayer(std::shared_ptr<Camera> camera, std::shared_ptr<Window> window,std::shared_ptr<RenderingLayer> renderingLayer);

        void init() override;

        void update() override;

        ~GUIMaterialLayer() override;
    private:
        void RenderMaterialEditor();
        void RenderLightEditor();
        std::shared_ptr<ModelInstance> selectedInstance;
        std::shared_ptr<Light> selectedLight;
        std::shared_ptr<Window> window;
        std::shared_ptr<RenderingLayer> renderingLayer;
        std::shared_ptr<Camera> camera;
        void loadOBJ(const char *objPath) const;
        char objBuffer[255];
        VisitMaterialValue visitor;

    };
}

#endif //GLPROJECT_GUIMATERIALLAYER_H
