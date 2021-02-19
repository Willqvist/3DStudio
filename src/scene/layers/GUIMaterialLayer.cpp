//
// Created by William Lundqvist on 2020-12-05.
//

#include "GUIMaterialLayer.h"
#include "../../window/Window.h"
#include "../../Core.h"
#include "../../loaders/ObjLoader.h"

GLProj::GUIMaterialLayer::GUIMaterialLayer(std::shared_ptr<Camera> camera, std::shared_ptr<Window> window, std::shared_ptr<RenderingLayer> renderingLayer) : camera(camera), window(window), renderingLayer(renderingLayer) {
    memset(objBuffer,0,255);
}

void GLProj::GUIMaterialLayer::init() {

}
static std::string selected = "";
static std::string selectedLightName = "";
static char buff[512];
bool open = true;
int menuItem = 0;
void GLProj::GUIMaterialLayer::update() {

    ImGui::SetNextWindowPos(ImVec2(0,window->getHeight()/2),ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(300,window->getHeight()/2),ImGuiCond_Once);
    ImGui::Begin("Editor",&open,ImGuiWindowFlags_MenuBar);
    if(ImGui::BeginMenuBar()) {
        if(ImGui::MenuItem("Object Editor",NULL,menuItem==0,menuItem!=0)) {
            menuItem = 0;
            memset(objBuffer,0,255);
        } else if(ImGui::MenuItem("Light Editor",NULL,menuItem==1,menuItem!=1)) {
            menuItem = 1;
            memset(objBuffer,0,255);
        }
        ImGui::EndMenuBar();
    }

    if(selected.length() == 0 && renderingLayer->getInstances().size() > 0) {
        selected = renderingLayer->getInstances()[0].name;
        selectedInstance = renderingLayer->getInstances()[0].instance;
    }

    switch(menuItem) {
        case 0 : RenderMaterialEditor(); break;
        case 1 : RenderLightEditor(); break;
    }

    ImGui::End();
}

void GLProj::GUIMaterialLayer::RenderLightEditor() {
    ImGui::Text("Light Editor");
    ImGui::SameLine();
    ImGui::PushItemWidth(-1);
    if(ImGui::BeginCombo("Light Instances",selectedLightName.c_str())) {
        for (GLProj::LightInstance &inst : this->renderingLayer->getLights()) {
            if(ImGui::Selectable(inst.name.c_str())) {
                selectedLightName = inst.name;
                this->selectedLight = inst.instance;
            }
        }
        ImGui::EndCombo();
    }
    ImGui::PopItemWidth();
    if(this->selectedLight != nullptr) {

        ImGui::Dummy(ImVec2(1,5));
        ImGui::Text("Light Settings for %s",selectedLightName.c_str());



        ImGui::Dummy(ImVec2(0,2));

        switch(selectedLight->type) {
            case POINT: {
                ImGui::Text("Type Point");
                visitor.name = "Position";
                ImGui::PushID(0);
                visitor.renderVector(selectedLight->position);
                ImGui::PopID();

                ImGui::PushItemWidth(52.f);
                ImGui::InputFloat("radius",&selectedLight->radius);
            } break;
            case DIRECTIONAL:  {
                ImGui::Text("Type Direction");
                visitor.name = "Direction";
                ImGui::PushID(0);
                visitor.renderVector(selectedLight->direction);
                ImGui::PopID();
                visitor.name = "Position";
                ImGui::PushID(1);
                visitor.renderVector(selectedLight->position);
                ImGui::PopID();
            }
        }

        ImVec4 lightCol(selectedLight->color.x,selectedLight->color.y,selectedLight->color.z,1);
        if(ImGui::ColorEdit4("Color",&lightCol.x)) {
            selectedLight->color = glm::vec3(lightCol.x,lightCol.y,lightCol.z);
        }

    }
}

void GLProj::GUIMaterialLayer::RenderMaterialEditor() {
    ImGui::Text("Object Editor");
    ImGui::SameLine();
    ImGui::PushItemWidth(-1);
    if(ImGui::BeginCombo("Instances",selected.c_str())) {
        for (GLProj::RenderingInstance &inst : this->renderingLayer->getInstances()) {
            if(ImGui::Selectable(inst.name.c_str())) {
                selected = inst.name;
                this->selectedInstance = inst.instance;
            }
        }
        ImGui::EndCombo();
    }
    ImGui::PopItemWidth();

    if(this->selectedInstance != nullptr) {
        std::shared_ptr<GLProj::Material> mat = this->selectedInstance->getMaterial();
        std::map<std::string, GLProj::MaterialVariable> values = mat->getValues();
        std::vector<GLProj::MaterialTexture> textures = mat->getTextures();
        this->visitor.material = mat;
        ImGui::Dummy(ImVec2(1,5));
        ImGui::Text("Settings for %s",selected.c_str());

        ImGui::BeginGroup();
        ImGui::Dummy(ImVec2(0,2));
        ImGui::Text("Load Model");
        ImGui::PushItemWidth(164.f);
        ImGui::InputTextWithHint("", "path", &objBuffer[0], 255);
        ImGui::PopItemWidth();
        ImGui::SameLine();
        if (ImGui::Button("Load .obj")) {
            this->loadOBJ(&objBuffer[0]);
        }
        ImGui::EndGroup();

        ImGui::Dummy(ImVec2(1,2));
        ImGui::Text("Transform");

        this->visitor.name = "Position";
        ImGui::PushID(0);
        this->visitor.renderVector(this->selectedInstance->getPosition());
            //selectedInstance->setPosition(selectedInstance->getPosition());
        ImGui::PopID();

        this->visitor.name = "Rotation";
        ImGui::PushID(1);
        this->visitor.renderVector(this->selectedInstance->getRotation());
        ImGui::PopID();

        this->visitor.name = "Scale";
        ImGui::PushID(2);
        this->visitor.renderVector(this->selectedInstance->getScale());
        ImGui::PopID();

        ImGui::Dummy(ImVec2(1,2));
        ImGui::Text("Textures");
        int id = 4;
        for(auto& tex : textures) {
            ImGui::Image((void*)(intptr_t)tex.texture->getId(),ImVec2(64,64));
            ImGui::SameLine();
            ImGui::BeginGroup();
            ImGui::Text(tex.name.c_str());
            ImGui::PushItemWidth(-1);
            ImGui::PushID(id);
            ImGui::InputTextWithHint("##","path",tex.texture->getPathBuffer(),TEXTURE_BUFFER_SIZE);
            ImGui::PopItemWidth();
            ImGui::PushID(0);
            if(ImGui::Button("Apply")) {
                std::shared_ptr<GLProj::Texture> loadedTex = GLProj::Texture::load(tex.texture->getPathBuffer(), GL_TEXTURE_2D);
                if(loadedTex) {
                    mat->set(tex.name, loadedTex);
                }
            }
            ImGui::PopID();
            ImGui::SameLine();
            ImGui::PushID(1);
            if(ImGui::Button("Remove")) {
                tex.texture->getPathBuffer()[0] = '\0';
                mat->set(tex.name, GLProj::Texture::white());
            }
            ImGui::PopID();
            ImGui::PopID();
            ImGui::EndGroup();
            id ++;
        }
        ImGui::Dummy(ImVec2(1,2));
        ImGui::Text("Variables");
        for(auto& value : values) {
            //value.second
            this->visitor.name = value.first;
            ImGui::PushID(textures.size()+1+id);
            std::visit(this->visitor, value.second);
            ImGui::PopID();
            id++;
        }
    }
}

void GLProj::GUIMaterialLayer::loadOBJ(const char *objPath) const {

    GLProj::ObjLoaderSettings settings = {
            true,
            true,
    };

    std::shared_ptr<GLProj::OBJ> obj = GLProj::ObjLoader::loadOBJ(objPath,settings);
    if(obj) {
        DEBUG("Opened " << objPath << "!");
        std::shared_ptr<GLProj::Model> model = GLProj::Model::create(obj);
        selectedInstance->setModel(model);
        selectedInstance->setPosition(0, 0, 0);
        selectedInstance->setRotation(0, 0, 0);
        selectedInstance->setScale(1, 1, 1);
        this->camera->setPosition(0,0,2);
        this->camera->lookAt(0,0,0);
        this->camera->update();
    }
}


GLProj::GUIMaterialLayer::~GUIMaterialLayer() {

}
