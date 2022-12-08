#include "MainEngine.h"

#include <glad/glad.h>

#include <imgui.h>
#include <imgui_impl/imgui_impl_glfw.h>
#include <imgui_impl/imgui_impl_opengl3.h>

#include <stb_image.h>

#include "LoggingMacros.h"
#include "Model.h"
#include "Nodes/ModelNode.h"

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

    glfwWindowHint(GLFW_SAMPLES, 4);

    if (InitializeWindow() != 0)
        return 1;

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        SPDLOG_ERROR("Failed to initialize GLAD!");
        return 1;
    }
    SPDLOG_DEBUG("Successfully initialized OpenGL loader!");

    stbi_set_flip_vertically_on_load(true);

    InitializeImGui(GLSLVersion);

    glClearColor(0.f, 0.f, 0.f, 1.f);

    return 0;
}

int32_t MainEngine::InitializeWindow()
{
    Window = glfwCreateWindow(640, 480, "Housing Estate", nullptr, nullptr);
    if (Window == nullptr)
    {
        SPDLOG_ERROR("Failed to create OpenGL Window");
        return 1;
    }

    glfwMakeContextCurrent(Window);
    glfwSwapInterval(false);
    return 0;
}

void MainEngine::GLFWErrorCallback(int Error, const char* Description)
{
    SPDLOG_ERROR("GLFW Error {}: {}", Error, Description);
}

int32_t MainEngine::MainLoop()
{
    auto StartProgramTimePoint = std::chrono::high_resolution_clock::now();
    float PreviousFrameSeconds = 0;

#ifdef DEBUG
    CheckGLErrors();
#endif

    while (!glfwWindowShouldClose(Window))
    {
        // TimeCalculation
        std::chrono::duration<float> TimeFromStart = std::chrono::high_resolution_clock::now() - StartProgramTimePoint;
        float Seconds = TimeFromStart.count();
        float DeltaSeconds = Seconds - PreviousFrameSeconds;
        PreviousFrameSeconds = Seconds;

        glClearDepth(1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glm::vec<2, int> CurrentResolution{};
        glfwMakeContextCurrent(Window);
        glfwGetFramebufferSize(Window, &CurrentResolution.x, &CurrentResolution.y);
        glViewport(0, 0, CurrentResolution.x, CurrentResolution.y);

        SceneRoot.Update(this, Seconds, DeltaSeconds);
        SceneRoot.CalculateWorldTransform();
        SceneRoot.Draw();

        Renderer.Draw();

        UpdateWidget(DeltaSeconds);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(Window);
        glfwPollEvents();
    }

    return 0;
}

void MainEngine::UpdateWidget(float DeltaSeconds)
{
    ImGui::Begin("Yet another 2D Engine");
    ImGui::Text("Framerate: %.3f (%.1f FPS)", DeltaSeconds, 1 / DeltaSeconds);
    ImGui::End();
}

MainEngine::MainEngine() : SceneRoot()
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

    ImGui_ImplGlfw_InitForOpenGL(Window, true);
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

    if (!Window)
        return;

    glfwDestroyWindow(Window);
    glfwTerminate();
}

void MainEngine::CheckGLErrors()
{
    GLenum Error;

    while ((Error = glGetError()) != GL_NO_ERROR)
        SPDLOG_ERROR("OpenGL Error: {}", Error);
}

void MainEngine::PrepareScene()
{
}
