#pragma once

#include <memory>

#include <cstdint>
#include <GLFW/glfw3.h>

#include "glm/vec3.hpp"
#include "Nodes/Node.h"
#include "glm/gtc/constants.hpp"

class MainEngine {
private:
    GLFWwindow* window;

    class CameraNode* currentCameraNode;
    Node sceneRoot;
    std::unique_ptr<class SpriteRenderer> renderer;

public:
    explicit MainEngine();
    virtual ~MainEngine();

    int32_t Init();
    void PrepareScene();
    int32_t MainLoop();

    GLFWwindow *GetWindow() const;

    Node &GetSceneRoot();

    friend class CameraNode;
private:
    void Stop();

    static void GLFWErrorCallback(int Error, const char* Description);
    int32_t InitializeWindow();
    void InitializeImGui(const char* GLSLVersion);
    void UpdateWidget(float DeltaSeconds);
    static  void CheckGLErrors();

    std::shared_ptr<class RigidbodyNode> CreateRigidbodyTile(const std::shared_ptr<class Sprite>& sprite);

    std::shared_ptr<class Map> CreateNodeMap();
};
