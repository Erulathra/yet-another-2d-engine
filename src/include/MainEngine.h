#pragma once

#include <memory>

#include <cstdint>
#include <GLFW/glfw3.h>

#include "glm/vec3.hpp"
#include "Nodes/Node.h"
#include "glm/gtc/constants.hpp"
#include "ModelRenderer.h"

class MainEngine {
private:
    GLFWwindow* Window;

    std::shared_ptr<class Camera> MainCamera;
    Node SceneRoot;
    ModelRenderer Renderer;

public:
    explicit MainEngine();
    virtual ~MainEngine();

    int32_t Init();
    void PrepareScene();
    int32_t MainLoop();

private:
    void Stop();

    static void GLFWErrorCallback(int Error, const char* Description);
    int32_t InitializeWindow();
    void InitializeImGui(const char* GLSLVersion);
    void UpdateWidget(float DeltaSeconds);
    static  void CheckGLErrors();
};
