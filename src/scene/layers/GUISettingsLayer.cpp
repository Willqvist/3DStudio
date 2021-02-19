//
// Created by William Lundqvist on 2020-12-05.
//

#include <imgui/imgui.h>
#include "GUISettingsLayer.h"
#include "../../loaders/ObjLoader.h"

GLProj::GUISettingsLayer::GUISettingsLayer(std::shared_ptr<Window> window,
                                           std::shared_ptr<Camera> camera,
                                           std::shared_ptr<RenderingLayer> renderingLayer)
                                           : window(window), camera(camera), renderingLayer(renderingLayer) {

}

void GLProj::GUISettingsLayer::init() {

}

void GLProj::GUISettingsLayer::update() {

    ImGui::SetNextWindowPos(ImVec2(0,0),ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(300,window->getHeight()/2),ImGuiCond_Once);

    ImGui::Begin("Settings",&open,ImGuiWindowFlags_MenuBar);
    if(ImGui::BeginMenuBar()) {
        if(ImGui::MenuItem("Camera Settings",NULL,menuItem==0,menuItem!=0)) {
            menuItem = 0;
        } else if(ImGui::MenuItem("Renderer Settings",NULL,menuItem==1,menuItem!=1)) {
            menuItem = 1;
        }
        ImGui::EndMenuBar();
    }
    switch(menuItem) {
        case 0: renderCameraSettings(); break;
        case 1: renderRendererSettings(); break;
    }

    ImGui::End();
}

static std::string selectedRenderName = "";

void GLProj::GUISettingsLayer::renderRendererSettings() {
    ImGui::Dummy(ImVec2(0,2));
    ImGui::Text("Shading types");
    ImGui::PushID(0);
    if(ImGui::BeginCombo("",selectedRenderName.c_str())) {

        if(ImGui::Selectable("Gourard Shading")) {
            selectedRenderName = "Gourard Shading";
            renderingLayer->setMode(GL_TRIANGLES);
            std::shared_ptr<Shader> gourard = Shader::create("res/vgourard.glsl","res/fgourard.glsl",shaderLocations);
            for(auto& inst : renderingLayer->getInstances()) {
                inst.instance->getMaterial()->setShader(gourard);
            }
        }

        if(ImGui::Selectable("Pong Shading")) {
            selectedRenderName = "Pong Shading";
            renderingLayer->setMode(GL_TRIANGLES);
            std::shared_ptr<Shader> pong = Shader::create("res/vshader.glsl","res/fshader.glsl",shaderLocations);
            for(auto& inst : renderingLayer->getInstances()) {
                inst.instance->getMaterial()->setShader(pong);
            }
        }

        if(ImGui::Selectable("Toon Shading")) {
            selectedRenderName = "Toon Shading";
            renderingLayer->setMode(GL_TRIANGLES);
            std::shared_ptr<Shader> toon = Shader::create("res/vshader.glsl","res/ftoon.glsl",shaderLocations);
            for(auto& inst : renderingLayer->getInstances()) {
                inst.instance->getMaterial()->setShader(toon);
            }
        }

        if(ImGui::Selectable("None")) {
            selectedRenderName = "None";
            renderingLayer->setMode(GL_LINES);
        }
        ImGui::EndCombo();
    }
    ImGui::PopID();
}
void GLProj::GUISettingsLayer::renderCameraSettings() {

    ImGui::Dummy(ImVec2(0,10));
    ImGui::BeginGroup();
    if(ImGui::RadioButton("Perspective", this->activePers)) {
        this->activePers = true;
        std::shared_ptr<GLProj::PerspectiveProjection> proj = std::make_shared<GLProj::PerspectiveProjection>(
                glm::radians(this->fov), this->window->getWidth() / (this->window->getHeight() * 1.0f), this->zNear, this->zFar);
        this->camera->setProjection(proj);
        ImGui::EndGroup();
        return;
    }
    //ImGui::SameLine();
    if(ImGui::RadioButton("Parallel",!this->activePers)) {
        this->activePers = false;
        std::shared_ptr<GLProj::ParallelProjection> proj = std::make_shared<GLProj::ParallelProjection>(this->window->getWidth()*1.0f,
                                                                                                        this->window->getHeight()*1.0f,
                                                                                                        this->topPar,
                                                                                                        this->zNear, this->farPar);
        proj->setScale(this->obliqueScale);
        proj->setAngle(this->obliqueAngle);
        proj->update();
        this->camera->setProjection(proj);
        ImGui::EndGroup();
        return;
    }

    if(this->activePers) {
        ImGui::Dummy(ImVec2(1,1));
        ImGui::Text("FOV");
        ImGui::PushItemWidth(-1);
        if(ImGui::SliderFloat("fov", &this->fov, 60, 120)) {
            std::shared_ptr<GLProj::PerspectiveProjection> proj = this->camera->getProjection<GLProj::PerspectiveProjection>();
            proj->setFov(glm::radians(this->fov));
            this->camera->update();
        }
        ImGui::PopItemWidth();
        ImGui::Dummy(ImVec2(1,1));

        ImGui::Text("far");
        ImGui::PushItemWidth(-1);
        if(ImGui::InputFloat("far",&this->zFar)) {
            std::shared_ptr<GLProj::PerspectiveProjection> proj = this->camera->getProjection<GLProj::PerspectiveProjection>();
            proj->setFar(glm::radians(this->zFar));
            this->camera->update();
        }
        ImGui::PopItemWidth();
        ImGui::Dummy(ImVec2(1,1));
    }

    if(!this->activePers) {
        ImGui::Dummy(ImVec2(1,1));
        ImGui::Text("Top");
        ImGui::PushItemWidth(-1);
        if(ImGui::InputFloat("top",&this->topPar)) {
            std::shared_ptr<GLProj::ParallelProjection> proj = this->camera->getProjection<GLProj::ParallelProjection>();
            proj->setTop(this->topPar);
            this->camera->update();
        }
        ImGui::PopItemWidth();

        ImGui::Dummy(ImVec2(1,1));
        ImGui::Text("Far");
        ImGui::PushItemWidth(-1);
        if(ImGui::InputFloat("Far",&this->farPar)) {
            std::shared_ptr<GLProj::ParallelProjection> proj = this->camera->getProjection<GLProj::ParallelProjection>();
            proj->setFar(this->farPar);
            this->camera->update();
        }
        ImGui::PopItemWidth();

        ImGui::Dummy(ImVec2(1,1));
        ImGui::Text("Oblique scale");
        ImGui::PushItemWidth(-1);
        if(ImGui::SliderFloat("Oblique scale", &this->obliqueScale, 0, 1)) {
            std::shared_ptr<GLProj::ParallelProjection> proj = this->camera->getProjection<GLProj::ParallelProjection>();
            proj->setScale(this->obliqueScale);
            this->camera->update();
        }
        ImGui::PopItemWidth();

        ImGui::Dummy(ImVec2(1,1));
        ImGui::Text("Oblique angle");
        ImGui::PushItemWidth(-1);
        if(ImGui::SliderFloat("Oblique angle", &this->obliqueAngle, 15, 75)) {
            std::shared_ptr<GLProj::ParallelProjection> proj = this->camera->getProjection<GLProj::ParallelProjection>();
            proj->setAngle(this->obliqueAngle);
            this->camera->update();
        }
        ImGui::PopItemWidth();
    }
    ImGui::EndGroup();
}


GLProj::GUISettingsLayer::~GUISettingsLayer() {

}