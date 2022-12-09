#include "Nodes/PlayerNode.h"
#include "Nodes/SpriteNode.h"
#include "MainEngine.h"
#include "GLFW/glfw3.h"
#include "LoggingMacros.h"

PlayerNode::PlayerNode(const std::shared_ptr<SpriteNode>& spriteNode) {
    AddChild(spriteNode);
    GetLocalTransform()->SetPosition(glm::vec3(0.f, 0.f, 1.f));
}

void PlayerNode::Update(struct MainEngine *engine, float seconds, float deltaSeconds) {
    Node::Update(engine, seconds, deltaSeconds);
    glm::vec3 input(GetMovementInput(engine), 0.f);
    glm::vec3 position = GetLocalTransform()->GetPosition() + input * deltaSeconds * 10.f;
    GetLocalTransform()->SetPosition(position);
}

glm::vec2 PlayerNode::GetMovementInput(MainEngine *engine) {
    glm::vec2 input{0};

    if (glfwGetKey(engine->GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
        input += glm::vec2(0, 1);
    if (glfwGetKey(engine->GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
        input += glm::vec2(0, -1);
    if (glfwGetKey(engine->GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
        input += glm::vec2(-1, 0);
    if (glfwGetKey(engine->GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
        input += glm::vec2(1, 0);

    return input;
}

