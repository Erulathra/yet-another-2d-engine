#include "Nodes/PlayerNode.h"
#include "Nodes/SpriteNode.h"
#include "MainEngine.h"
#include "GLFW/glfw3.h"
#include "LoggingMacros.h"

#include "Nodes/CollisionShapes/CollisionShapeFactory.h"

PlayerNode::PlayerNode()
: RigidbodyNode(CollisionShapeFactory::CreateFactory()->CreateCircleCollisionShape(0.46f)){
    GetLocalTransform()->SetPosition(glm::vec3(0.f, 0.f, 2.f));
}

void PlayerNode::Update(struct MainEngine *engine, float seconds, float deltaSeconds) {
    RigidbodyNode::Update(engine, seconds, deltaSeconds);

    glm::vec2 input = GetMovementInput(engine);

    if (glm::length(GetVelocity()) < 5.f && glm::length(input) > 0 )
        SetAcceleration(input * 50.f);
    else
        SetAcceleration(GetVelocity() * -10.f);
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

