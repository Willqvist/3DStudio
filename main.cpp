#include <chrono>
#include "src/window/Window.h"
#include "src/window/Input.h"
#include "src/scene/Scene.h"
#include "src/graphics/Renderer.h"
#include <thread>
int main() {
    std::shared_ptr<GLProj::Window> window = GLProj::Window::create(1080,720,"3D Studio",true);
    window->addListener(std::make_shared<GLProj::Input>());

    GLProj::Scene s = GLProj::Scene(window);
    GLProj::Renderer::init(window);

    while(window->update()) {

        if(!window->isSleeping())
            s.update();

        window->clear();
        GLProj::Renderer::clear();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    window->dispose();

    return 0;
}



