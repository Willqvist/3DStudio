cmake_minimum_required(VERSION 3.15)
project(GLProject)
set(CMAKE_CXX_STANDARD 17)
add_compile_definitions(DEBUG_ENABLE)
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

add_subdirectory(Dependencies/GLM/glm)
add_subdirectory(Dependencies/GLFW)
add_subdirectory(Dependencies/GLAD)
file(GLOB IMGUI
        "Dependencies/IMGUI/imgui/*.h"
        "Dependencies/IMGUI/imgui/*.cpp"
        )


file(GLOB loaders
        "src/loaders/*"
        )

add_executable(GLProject ${IMGUI} main.cpp ${loaders} Dependencies/STB/stb_image.h src/window/Window.h src/window/Window.cpp src/platform/glfw/WindowGLFW.cpp src/platform/glfw/WindowGLFW.h src/model/Model.cpp src/model/Model.h src/model/ModelInstance.cpp src/model/ModelInstance.h src/Core.h src/graphics/Shader.h src/platform/opengl/ShaderOpenGL.cpp src/platform/opengl/ShaderOpenGL.h src/graphics/Shader.cpp src/platform/glfw/GLFWWindowEventHandler.h src/graphics/Renderer.cpp src/graphics/Renderer.h src/camera/Camera.cpp src/camera/Camera.h src/window/Input.h src/window/Input.cpp src/scene/Scene.cpp src/scene/Scene.h src/model/Mesh.cpp src/model/Mesh.h src/Moveable.h src/camera/Projection.h src/tools/cvec3.h src/graphics/Texture.cpp src/graphics/Texture.h Dependencies/STB/stb_image.h src/scene/Layer.h src/graphics/Material.cpp src/graphics/Material.h src/loaders/TextureLoader.cpp src/loaders/TextureLoader.h src/scene/layers/RenderingLayer.cpp src/scene/layers/RenderingLayer.h src/scene/layers/GUISettingsLayer.cpp src/scene/layers/GUISettingsLayer.h src/scene/layers/InputLayer.cpp src/scene/layers/InputLayer.h src/scene/layers/GUIMaterialLayer.cpp src/scene/layers/GUIMaterialLayer.h src/graphics/Light.h src/graphics/FBO.cpp src/graphics/FBO.h)
target_link_libraries(GLProject opengl32)
target_link_libraries(GLProject glad)
target_link_libraries(GLProject glm)
target_link_libraries(GLProject glfw)
target_include_directories(GLProject PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Dependencies/IMGUI)
target_include_directories(GLProject PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Dependencies/STB)
