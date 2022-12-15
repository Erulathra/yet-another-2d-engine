#include "MainEngine.h"

#include <glad/glad.h>

#include <imgui.h>
#include <imgui_impl/imgui_impl_glfw.h>
#include <imgui_impl/imgui_impl_opengl3.h>

#include <stb_image.h>

#include "LoggingMacros.h"
#include "SpriteRenderer.h"
#include "ShaderWrapper.h"
#include "Sprite.h"

#include "Nodes/CollisionShapes/CollisionShapeFactory.h"
#include "Nodes/RigidbodyNode.h"
#include "Nodes/CameraNode.h"
#include "Nodes/SpriteNode.h"
#include "Nodes/Map.h"
#include "Nodes/PlayerNode.h"

int32_t MainEngine::Init()
{
    glfwSetErrorCallback(MainEngine::GLFWErrorCallback);
    if (!glfwInit())
        return 1;

    const char* GLSLVersion = "#version 430";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only

    if (InitializeWindow() != 0)
        return 1;

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        SPDLOG_ERROR("Failed to initialize GLAD!");
        return 1;
    }
    SPDLOG_DEBUG("Successfully initialized OpenGL loader!");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    stbi_set_flip_vertically_on_load(true);

    InitializeImGui(GLSLVersion);

    glClearColor(0.f, 0.f, 0.f, 1.f);

    renderer = std::make_unique<SpriteRenderer>("res/textures/TileMap.png", 32);

    return 0;
}

int32_t MainEngine::InitializeWindow()
{
    window = glfwCreateWindow(640, 480, "Yet another 2D Engine", nullptr, nullptr);
    if (window == nullptr)
    {
        SPDLOG_ERROR("Failed to create OpenGL Window");
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(false);
    return 0;
}

void MainEngine::GLFWErrorCallback(int Error, const char* Description)
{
    SPDLOG_ERROR("GLFW Error {}: {}", Error, Description);
}

int32_t MainEngine::MainLoop()
{
    auto startProgramTimePoint = std::chrono::high_resolution_clock::now();
    float previousFrameSeconds = 0;

#ifdef DEBUG
    CheckGLErrors();
#endif

    while (!glfwWindowShouldClose(window))
    {
        // TimeCalculation
        std::chrono::duration<float> timeFromStart = std::chrono::high_resolution_clock::now() - startProgramTimePoint;
        float seconds = timeFromStart.count();
        float deltaSeconds = seconds - previousFrameSeconds;
        previousFrameSeconds = seconds;

        glClearDepth(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glm::vec<2, int> currentResolution{};
        glfwMakeContextCurrent(window);
        glfwGetFramebufferSize(window, &currentResolution.x, &currentResolution.y);
        glViewport(0, 0, currentResolution.x, currentResolution.y);

        sceneRoot.Update(this, seconds, deltaSeconds);
        sceneRoot.CalculateWorldTransform();
        sceneRoot.Draw();

        renderer->Draw();

        if (currentCameraNode == nullptr)
            SPDLOG_ERROR("No active CameraNode");

        UpdateWidget(deltaSeconds);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}

void MainEngine::UpdateWidget(float DeltaSeconds)
{
    ImGui::Begin("Yet another 2D Engine");
    ImGui::Text("Framerate: %.3f (%.1f FPS)", DeltaSeconds, 1 / DeltaSeconds);

    float cameraScale = currentCameraNode->GetScale();
    ImGui::SliderFloat("Camera Scale", &cameraScale, 1.f, 256.f);
    currentCameraNode->SetScale(cameraScale);

    ImGui::End();
}

MainEngine::MainEngine()
: sceneRoot(), currentCameraNode(nullptr)
{
}

void MainEngine::InitializeImGui(const char* GLSLVersion)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(GLSLVersion);

    // Setup style
    ImGui::StyleColorsDark();
}

MainEngine::~MainEngine()
{
    Stop();
}

void MainEngine::Stop()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    if (!window)
        return;

    glfwDestroyWindow(window);
    glfwTerminate();
}

void MainEngine::CheckGLErrors()
{
    GLenum error;

    while ((error = glGetError()) != GL_NO_ERROR)
        SPDLOG_ERROR("OpenGL error: {}", error);
}

std::shared_ptr<Map> CreateNodeMap(SpriteRenderer* renderer);

void MainEngine::PrepareScene() {
    auto map = CreateNodeMap(renderer.get());
    glm::vec2 mapSize = map->GetSize();
    map->GetLocalTransform()->SetPosition(glm::vec3(-mapSize.x / 2 + 0.5f, -mapSize.y / 2 + 0.5f, 0));
    sceneRoot.AddChild(map);

    auto ballSprite = std::make_shared<Sprite>(glm::vec<2, int>(0, 2));
    auto playerSpriteNode = std::make_shared<SpriteNode>(ballSprite, renderer.get());
    auto playerNode = std::make_shared<PlayerNode>();
    auto cameraNode = std::make_shared<CameraNode>(this);
    cameraNode->MakeCurrent();
    playerNode->AddChild(playerSpriteNode);
    playerNode->AddChild(cameraNode);
    sceneRoot.AddChild(playerNode);

    sceneRoot.CalculateWorldTransform();
}

std::shared_ptr<Map> CreateNodeMap(SpriteRenderer* renderer) {
    auto brownBricksSprite = std::make_shared<Sprite>(glm::vec<2, int>(0, 1));
    auto bricksSprite = std::make_shared<Sprite>(glm::vec<2, int>(1, 1));

    auto brick = std::make_shared<SpriteNode>(bricksSprite, renderer);
    auto path = std::make_shared<SpriteNode>(brownBricksSprite, renderer);

    auto collisionShapeFactory = CollisionShapeFactory::CreateFactory()->CreateRectangleCollisionShape(1, 1);
    auto brickRigidBody = std::make_shared<RigidbodyNode>(collisionShapeFactory);
    brickRigidBody->AddChild(brick);
    brickRigidBody->SetIsKinematic(true);


    std::map<char, Node *> nodesMap;
    nodesMap['#'] = brickRigidBody.get();
    nodesMap[' '] = path.get();
    return std::make_shared<Map>("res/other/map", nodesMap);
}

GLFWwindow *MainEngine::GetWindow() const {
    return window;
}

Node &MainEngine::GetSceneRoot() {
    return sceneRoot;
}
