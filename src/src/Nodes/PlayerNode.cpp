#include "Nodes/PlayerNode.h"
#include "Nodes/SpriteNode.h"
#include "MainEngine.h"
#include "GLFW/glfw3.h"
#include "LoggingMacros.h"

#include "Nodes/CollisionShapes/CollisionShapeFactory.h"

PlayerNode::PlayerNode()
: RigidbodyNode(CollisionShapeFactory::CreateFactory()->CreateCircleCollisionShape(0.5f)){
    GetLocalTransform()->SetPosition(glm::vec3(0.f, 0.f, 2.f));
}

void PlayerNode::Update(struct MainEngine *engine, float seconds, float deltaSeconds) {
    RigidbodyNode::Update(engine, seconds, deltaSeconds);

    glm::vec2 input = GetMovementInput(engine);

    glm::vec2 newAcceleration;

    if (std::abs(GetVelocity().x) < 5.f && std::abs(input.x) > 0 )
        newAcceleration.x = input.x * 50.f;
    else
        newAcceleration.x = GetVelocity().x * -10.f;

    newAcceleration.y = gravityAcceleration;

    SetAcceleration(newAcceleration);
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

