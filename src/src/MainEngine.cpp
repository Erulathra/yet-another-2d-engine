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
#include "Nodes/TimerNode.h"
#include "Nodes/ParallaxNode.h"

int32_t MainEngine::Init() {
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

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        SPDLOG_ERROR("Failed to initialize GLAD!");
        return 1;
    }
    SPDLOG_DEBUG("Successfully initialized OpenGL loader!");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    stbi_set_flip_vertically_on_load(true);

    InitializeImGui(GLSLVersion);

    glClearColor(0.929f, 0.706f, 0.631f, 1.f);

    renderer = std::make_unique<SpriteRenderer>("res/textures/TileMap.png", 8);

    return 0;
}

int32_t MainEngine::InitializeWindow() {
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    window = glfwCreateWindow(640, 480, "Yet another 2D Engine", nullptr, nullptr);
    if (window == nullptr) {
        SPDLOG_ERROR("Failed to create OpenGL Window");
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(false);
    return 0;
}

void MainEngine::GLFWErrorCallback(int Error, const char* Description) {
    SPDLOG_ERROR("GLFW Error {}: {}", Error, Description);
}

int32_t MainEngine::MainLoop() {
    sceneRoot.Start(this);

    auto startProgramTimePoint = std::chrono::high_resolution_clock::now();
    float previousFrameSeconds = 0;

#ifdef DEBUG
    CheckGLErrors();
#endif

    while (!glfwWindowShouldClose(window)) {
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

void MainEngine::UpdateWidget(float DeltaSeconds) {
    ImGui::Begin("Yet another 2D Engine");
    ImGui::Text("Framerate: %.3f (%.1f FPS)", DeltaSeconds, 1 / DeltaSeconds);

    float cameraScale = currentCameraNode->GetScale();
    ImGui::DragFloat("Camera Scale", &cameraScale, 0.5f, 1.f, 256.f);
    currentCameraNode->SetScale(cameraScale);

    auto Player = dynamic_cast<PlayerNode*>(sceneRoot.GetChild([](Node* node) -> bool {
        return dynamic_cast<PlayerNode*>(node) != nullptr;
    }));

    constinit static float jumpHeight = 2.f;
    constinit static float jumpDistance = 4.0f;
    constinit static float fallGravityFactor = 1.4f;
    constinit static float buttonPressGravityFactor = 0.5f;
    constinit static float playerSpeed = 7.f;

    ImGui::DragFloat("Jump Height", &jumpHeight, 0.1f, 0.5f, 32.f);
    ImGui::DragFloat("Jump Distance", &jumpDistance, 0.1f, 0.5f, 32.f);

    ImGui::Text("g: %.1f, v0: %.1f", Player->GetGravityAcceleration(), Player->GetStartJumpVelocity());
    ImGui::Separator();

    ImGui::DragFloat("Fall gravity factor", &fallGravityFactor, 0.05f, 0.1f, 1.f);
    ImGui::DragFloat("Short Jump factor", &buttonPressGravityFactor, 0.05f, 0.1f, 1.f);
    ImGui::Separator();

    ImGui::DragFloat("PlayerSpeed", &playerSpeed, 0.05f, 0.1f, 64.f);

    Player->SetPlayerSpeed(playerSpeed);
    Player->SetFallGravityFactor(fallGravityFactor);
    Player->SetButtonPressJumpGravityFactor(buttonPressGravityFactor);
    Player->SetJumpParameters(jumpHeight, jumpDistance);

    ImGui::End();
}

MainEngine::MainEngine()
        : sceneRoot(), currentCameraNode(nullptr) {
}

void MainEngine::InitializeImGui(const char* GLSLVersion) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(GLSLVersion);

    // Setup style
    ImGui::StyleColorsDark();
}

MainEngine::~MainEngine() {
    Stop();
}

void MainEngine::Stop() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    if (!window)
        return;

    glfwDestroyWindow(window);
    glfwTerminate();
}

void MainEngine::CheckGLErrors() {
    GLenum error;

    while ((error = glGetError()) != GL_NO_ERROR)
        SPDLOG_ERROR("OpenGL error: {}", error);
}

void MainEngine::PrepareScene() {
    auto map = CreateNodeMap();
    glm::vec2 mapSize = map->GetSize();
    map->GetLocalTransform()->SetPosition(glm::vec3(-mapSize.x / 2 + 0.5f, -mapSize.y / 2 + 0.5f, 0));
    sceneRoot.AddChild(map);

    auto backgroundOneParallax = std::make_shared<ParallaxNode>(0.2f);
    backgroundOneParallax->GetLocalTransform()->SetPosition({0.f, 0.f, -10.f});
    auto backgroundOne = CreateNodeMapBackground("res/other/background_one");
    glm::vec2 backgroundSize = backgroundOne->GetSize();
    backgroundOne->GetLocalTransform()->SetPosition(glm::vec3(-mapSize.x / 2 + 0.5f, -mapSize.y / 2 + 0.5f, 0));
    backgroundOneParallax->AddChild(backgroundOne);
    sceneRoot.AddChild(backgroundOneParallax);

    auto backgroundTwoParallax = std::make_shared<ParallaxNode>(0.4f);
    backgroundTwoParallax->GetLocalTransform()->SetPosition({0.f, 0.f, -20.f});
    auto backgroundTwo = CreateNodeMapBackground("res/other/background_two");
    backgroundSize = backgroundTwo->GetSize();
    backgroundTwo->GetLocalTransform()->SetPosition(glm::vec3(-mapSize.x / 2 + 0.5f, -mapSize.y / 2 + 0.5f, 0));
    backgroundTwoParallax->AddChild(backgroundTwo);
    sceneRoot.AddChild(backgroundTwoParallax);

    auto playerNode = std::make_shared<PlayerNode>(this, renderer.get());
    playerNode->GetLocalTransform()->SetPosition({-20.f, 0.f, 2.f});
    sceneRoot.AddChild(playerNode);

    sceneRoot.CalculateWorldTransform();
}

std::shared_ptr<Map> MainEngine::CreateNodeMap() {
    auto cornerSprite = std::make_shared<Sprite>(glm::vec<2, int>(0, 0));
    auto horizontalSprite = std::make_shared<Sprite>(glm::vec<2, int>(1, 0));
    auto verticalSprite = std::make_shared<Sprite>(glm::vec<2, int>(2, 0));
    auto stoneSprite = std::make_shared<Sprite>(glm::vec<2, int>(3, 1));
    auto insideCornerSprite = std::make_shared<Sprite>(glm::vec<2, int>(3, 0));

    auto stoneTileNode = std::make_shared<SpriteNode>(stoneSprite, renderer.get());
    auto horizontalTileNode = CreateRigidbodyTile(horizontalSprite);
    auto revertedHorizontalTileNode = CreateRigidbodyTile(horizontalSprite);
    revertedHorizontalTileNode->GetLocalTransform()->SetRotation(glm::quat(glm::radians(180.f), glm::vec3(1.f, 0.f, 0.f)));

    auto verticalTileNode = CreateRigidbodyTile(verticalSprite);
    auto revertedVerticalTileNode = CreateRigidbodyTile(verticalSprite);
    revertedVerticalTileNode->GetLocalTransform()->SetRotation(glm::quat(glm::radians(180.f), glm::vec3(0.f, 1.f, 0.f)));

    auto leftUpTileNode = CreateRigidbodyTile(cornerSprite);
    auto rightUpTileNode = CreateRigidbodyTile(cornerSprite);
    rightUpTileNode->GetLocalTransform()->SetRotation(glm::quat(glm::radians(180.f), glm::vec3(0.f, 1.f, 0.f)));

    auto rightDownTileNode = CreateRigidbodyTile(cornerSprite);
    rightDownTileNode->GetLocalTransform()->SetRotation(glm::quat({0.f, 0.f, glm::radians(180.f)}));
    auto leftDownTileNode = CreateRigidbodyTile(cornerSprite);
    leftDownTileNode->GetLocalTransform()->SetRotation(glm::quat({glm::pi<float>(), 0.f, 0.f}));

    auto innerDownRightNode = std::make_shared<SpriteNode>( insideCornerSprite, renderer.get());
    auto innerDownLeftNode = std::make_shared<SpriteNode>( insideCornerSprite, renderer.get());
    innerDownLeftNode->GetLocalTransform()->SetRotation(glm::quat({0.f, glm::radians(180.f), 0.f}));

    auto innerUpRightNode = std::make_shared<SpriteNode>( insideCornerSprite, renderer.get());
    innerUpRightNode->GetLocalTransform()->SetRotation(glm::quat({glm::radians(180.f), glm::radians(180.f), 0.f}));

    auto innerUpLeftNode = std::make_shared<SpriteNode>( insideCornerSprite, renderer.get());
    innerUpLeftNode->GetLocalTransform()->SetRotation(glm::quat({glm::radians(180.f), 0.f, 0.f}));

    std::map<char, Node*> nodesMap;
    nodesMap['H'] = horizontalTileNode.get();
    nodesMap['h'] = revertedHorizontalTileNode.get();
    nodesMap['v'] = verticalTileNode.get();
    nodesMap['V'] = revertedVerticalTileNode.get();
    nodesMap['R'] = rightUpTileNode.get();
    nodesMap['L'] = leftUpTileNode.get();
    nodesMap['r'] = rightDownTileNode.get();
    nodesMap['l'] = leftDownTileNode.get();
    nodesMap['}'] = innerDownRightNode.get();
    nodesMap['{'] = innerDownLeftNode.get();
    nodesMap[']'] = innerUpLeftNode.get();
    nodesMap['['] = innerUpRightNode.get();
    nodesMap[' '] = nullptr;
    nodesMap['#'] = stoneTileNode.get();
    return std::make_shared<Map>("res/other/map", nodesMap);
}

std::shared_ptr<RigidbodyNode> MainEngine::CreateRigidbodyTile(const std::shared_ptr<Sprite>& sprite)
{
    auto collisionShapeFactory = CollisionShapeFactory::CreateFactory()->CreateRectangleCollisionShape(1, 1);

    auto spriteNode = std::make_shared<SpriteNode>(sprite, renderer.get());
    auto rigidbodyNode = std::make_shared<RigidbodyNode>(collisionShapeFactory);
    rigidbodyNode->AddChild(spriteNode);
    rigidbodyNode->SetIsKinematic(true);

    return rigidbodyNode;
}

std::shared_ptr<class Map> MainEngine::CreateNodeMapBackground(std::string path) {
    auto cornerSprite = std::make_shared<Sprite>(glm::vec<2, int>(0, 0));
    auto horizontalSprite = std::make_shared<Sprite>(glm::vec<2, int>(1, 0));
    auto verticalSprite = std::make_shared<Sprite>(glm::vec<2, int>(2, 0));
    auto stoneSprite = std::make_shared<Sprite>(glm::vec<2, int>(3, 1));
    auto insideCornerSprite = std::make_shared<Sprite>(glm::vec<2, int>(3, 0));

    auto stalactiteBase = std::make_shared<Sprite>(glm::ivec2(1, 1));
    auto stalactiteTop = std::make_shared<Sprite>(glm::ivec2(0, 1));
    auto stalactiteCenter = std::make_shared<Sprite>(glm::ivec2(2, 1));

    auto stoneTileNode = std::make_shared<SpriteNode>(stoneSprite, renderer.get());
    auto horizontalTileNode = std::make_shared<SpriteNode>(horizontalSprite, renderer.get());
    auto revertedHorizontalTileNode = std::make_shared<SpriteNode>(horizontalSprite, renderer.get());
    revertedHorizontalTileNode->GetLocalTransform()->SetRotation(glm::quat(glm::radians(180.f), glm::vec3(1.f, 0.f, 0.f)));

    auto verticalTileNode = std::make_shared<SpriteNode>(verticalSprite, renderer.get());
    auto revertedVerticalTileNode = std::make_shared<SpriteNode>(verticalSprite, renderer.get());
    revertedVerticalTileNode->GetLocalTransform()->SetRotation(glm::quat(glm::radians(180.f), glm::vec3(0.f, 1.f, 0.f)));

    auto leftUpTileNode = std::make_shared<SpriteNode>(cornerSprite, renderer.get());
    auto rightUpTileNode = std::make_shared<SpriteNode>(cornerSprite, renderer.get());
    rightUpTileNode->GetLocalTransform()->SetRotation(glm::quat(glm::radians(180.f), glm::vec3(0.f, 1.f, 0.f)));

    auto rightDownTileNode = std::make_shared<SpriteNode>(cornerSprite, renderer.get());
    rightDownTileNode->GetLocalTransform()->SetRotation(glm::quat({0.f, 0.f, glm::radians(180.f)}));
    auto leftDownTileNode = std::make_shared<SpriteNode>(cornerSprite, renderer.get());
    leftDownTileNode->GetLocalTransform()->SetRotation(glm::quat({glm::pi<float>(), 0.f, 0.f}));

    auto innerDownRightNode = std::make_shared<SpriteNode>( insideCornerSprite, renderer.get());
    auto innerDownLeftNode = std::make_shared<SpriteNode>( insideCornerSprite, renderer.get());
    innerDownLeftNode->GetLocalTransform()->SetRotation(glm::quat({0.f, glm::radians(180.f), 0.f}));

    auto innerUpRightNode = std::make_shared<SpriteNode>( insideCornerSprite, renderer.get());
    innerUpRightNode->GetLocalTransform()->SetRotation(glm::quat({glm::radians(180.f), glm::radians(180.f), 0.f}));

    auto innerUpLeftNode = std::make_shared<SpriteNode>( insideCornerSprite, renderer.get());
    innerUpLeftNode->GetLocalTransform()->SetRotation(glm::quat({glm::radians(180.f), 0.f, 0.f}));

    auto stalactiteTopNode = std::make_shared<SpriteNode>(stalactiteTop, renderer.get());
    auto flippedStalactiteTopNode = std::make_shared<SpriteNode>(stalactiteTop, renderer.get());
    flippedStalactiteTopNode->GetLocalTransform()->SetRotation(glm::quat({glm::pi<float>(), 0.f, 0.f}));

    auto stalactiteCenterNode = std::make_shared<SpriteNode>(stalactiteCenter, renderer.get());
    auto flippedStalactiteCenterNode = std::make_shared<SpriteNode>(stalactiteCenter, renderer.get());
    flippedStalactiteCenterNode->GetLocalTransform()->SetRotation(glm::quat({glm::pi<float>(), 0.f, 0.f}));

    auto stalactiteBaseNode = std::make_shared<SpriteNode>(stalactiteBase, renderer.get());
    auto flippedStalactiteBaseNode = std::make_shared<SpriteNode>(stalactiteBase, renderer.get());
    flippedStalactiteBaseNode->GetLocalTransform()->SetRotation(glm::quat({glm::pi<float>(), 0.f, 0.f}));

    std::map<char, Node*> nodesMap;
    nodesMap['H'] = horizontalTileNode.get();
    nodesMap['h'] = revertedHorizontalTileNode.get();
    nodesMap['v'] = verticalTileNode.get();
    nodesMap['V'] = revertedVerticalTileNode.get();
    nodesMap['R'] = rightUpTileNode.get();
    nodesMap['L'] = leftUpTileNode.get();
    nodesMap['r'] = rightDownTileNode.get();
    nodesMap['l'] = leftDownTileNode.get();
    nodesMap['}'] = innerDownRightNode.get();
    nodesMap['{'] = innerDownLeftNode.get();
    nodesMap[']'] = innerUpLeftNode.get();
    nodesMap['['] = innerUpRightNode.get();
    nodesMap[' '] = nullptr;
    nodesMap['#'] = stoneTileNode.get();
    nodesMap['1'] = stalactiteTopNode.get();
    nodesMap['2'] = stalactiteCenterNode.get();
    nodesMap['3'] = stalactiteBaseNode.get();
    nodesMap['7'] = flippedStalactiteTopNode.get();
    nodesMap['8'] = flippedStalactiteCenterNode.get();
    nodesMap['9'] = flippedStalactiteBaseNode.get();

    return std::make_shared<Map>(path, nodesMap);
}

GLFWwindow* MainEngine::GetWindow() const {
    return window;
}

Node& MainEngine::GetSceneRoot() {
    return sceneRoot;
}

CameraNode* MainEngine::GetCurrentCameraNode() {
    return currentCameraNode;
}

void MainEngine::SetCurrentCameraNode(CameraNode* currentCameraNode) {
    MainEngine::currentCameraNode = currentCameraNode;
}

