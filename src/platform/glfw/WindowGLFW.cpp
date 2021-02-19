//
// Created by William Lundqvist on 2020-11-02.
//

#include <glad/glad.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>
#include <glm/common.hpp>
#include "WindowGLFW.h"
#include "../../Core.h"
#include "GLFWWindowEventHandler.h"
void onErrorCallback(int error, const char *description);

void APIENTRY glDebugHandler(GLenum source, GLenum type, unsigned int id, GLenum severity,
                            GLsizei length, const char *message, const void *userParam);

GLProj::WindowGLFW::WindowGLFW(int width, int height,const std::string &title, bool vsync) : Window(width, height, title, vsync) {
    char errorCode[1024];
    const char** outError = reinterpret_cast<const char **>(&errorCode);
    if(!glfwInit()) {
        glfwGetError(outError);
        DEBUG("Error Initializing GLFW: error (" << outError << ")");
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#ifdef DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif
    window = glfwCreateWindow(width,height,title.c_str(),NULL,NULL);
    if(!window) {
        glfwTerminate();
        glfwGetError(outError);
        DEBUG("Error Initializing GLFW Window: error (" << outError << ")");
        exit(EXIT_FAILURE);
    }

    glfwSetWindowUserPointer(window, reinterpret_cast<void*>(this));
    glfwMakeContextCurrent(window);
    glfwSwapInterval(vsync);

    //init glad
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    if(!status)
    {
        DEBUG("Error initializing GLAD");
        exit(EXIT_FAILURE);
    }

    glViewport(0,0,width,height);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_CCW);

    glPolygonMode(GL_FRONT, GL_LINE);
    glPolygonMode(GL_BACK, GL_LINE);

    setupListeners();

    IMGUI_CHECKVERSION();

    //creating and initializing Dear ImGui.
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 150");

#ifdef DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(glDebugHandler, nullptr);
    //glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#endif

}

bool GLProj::WindowGLFW::update() {
    if(glfwWindowShouldClose(window)) {
        return false;
    }
    glfwPollEvents();

    //updating ImGui
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    return true;
}

void GLProj::WindowGLFW::clear() {

    //clearing imgui.
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    //clearing glfw and opengl rendering.
    glfwSwapBuffers(window);
    glClearColor(bgColor.r,bgColor.g,bgColor.b,bgColor.a);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    //clearing listener data
    for(auto listener: listeners) {
        listener->onClear();
    }
}

void GLProj::WindowGLFW::dispose() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void GLProj::WindowGLFW::setupListeners() {
    glfwSetKeyCallback(window,GLFWWindowEventHandler::OnKeyCallback);
    glfwSetMouseButtonCallback(window,GLFWWindowEventHandler::MouseButtonCallback);
    glfwSetCursorPosCallback(window,GLFWWindowEventHandler::CursorPosCallback);
    glfwSetWindowSizeCallback(window,GLFWWindowEventHandler::WindowSizeCallback);
    glfwSetErrorCallback(onErrorCallback);
}

void GLProj::WindowGLFW::resize(int width,int height) {
    if(glfwGetCurrentContext() != window) {
        return;
    }
    if(width == 0 && height == 0) {
        this->sleeping = true;
    }
    else {
        this->sleeping = false;
    }
    this->width = width;
    this->height = height;
    glViewport(0,0,width,height);
}

void GLProj::WindowGLFW::mouseEnabled(bool enabled) {
    glfwSetInputMode(window, GLFW_CURSOR, enabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

void onErrorCallback(int error, const char *description) {
    DEBUGERR("error[" << error << "]: " << description);
}

void APIENTRY glDebugHandler(GLenum source, GLenum type, unsigned int id, GLenum severity,
                             GLsizei length, const char *message, const void *userParam) {

    if(id == 131169 || id == 131185 || id == 131218 || id == 131204)
        return;

    std::cerr << "GL ERROR["<< id <<"] " << std::endl;
    std::cerr << "\t" <<message << std::endl;
    std::cerr << "\tSource: ";
    switch(source) {
        case GL_DEBUG_SOURCE_API: std::cerr << "Api"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM: std::cerr << "Window"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cerr << "Shader"; break;
        case GL_DEBUG_SOURCE_APPLICATION: std::cerr << "Application"; break;
        case GL_DEBUG_SOURCE_OTHER: std::cerr << "Other"; break;
    }
    std::cerr << std::endl;

    std::cerr << "\tType: ";
    switch(type) {
        case GL_DEBUG_TYPE_ERROR: std::cerr << "Error"; break;
        case GL_DEBUG_TYPE_PERFORMANCE: std::cerr << "Performance"; break;
        case GL_DEBUG_TYPE_PORTABILITY: std::cerr << "Portability"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cerr << "Deprecated"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: std::cerr << "Undefined"; break;
        case GL_DEBUG_TYPE_OTHER: std::cerr << "Other"; break;
    }
    std::cerr << std::endl;
    if(severity == GL_DEBUG_SEVERITY_HIGH) {
        exit(id);
    }

}