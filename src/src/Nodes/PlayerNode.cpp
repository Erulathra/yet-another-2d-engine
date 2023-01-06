#include "Nodes/PlayerNode.h"
#include "Nodes/SpriteNode.h"
#include "MainEngine.h"
#include "GLFW/glfw3.h"
#include "LoggingMacros.h"

#include "Nodes/CollisionShapes/CollisionShapeFactory.h"
#include "Nodes/SpriteArrayNode.h"
#include "Nodes/CameraNode.h"
#include "Sprite.h"

PlayerNode::PlayerNode(MainEngine* engine, SpriteRenderer* renderer)
: RigidbodyNode(CollisionShapeFactory::CreateFactory()->CreateCircleCollisionShape(0.5f)) {
    playerSpeed = 7.f;
    fallGravityFactor = 0.8f;
    buttonPressJumpGravityFactor = 0.5f;
    SetJumpParameters(2.f, 0.5f);

    auto playerSpriteArrayNode = CreatePlayerSprite(engine, renderer);
    playerSprite = playerSpriteArrayNode;
    AddChild(playerSpriteArrayNode);

    auto cameraNode = std::make_shared<CameraNode>(engine);
    cameraNode->MakeCurrent();
    cameraNode->GetLocalTransform()->SetPosition({0.f, 2.f, 20.f});
    AddChild(cameraNode);

    auto jumpTrigger = std::make_shared<RigidbodyNode>(CollisionShapeFactory::CreateFactory()->CreateRectangleCollisionShape(0.15f, 0.7f));
    jumpTrigger->GetLocalTransform()->SetPosition({0.f, -0.5, 0.f});
    jumpTrigger->SetIsTrigger(true);
    AddChild(jumpTrigger);

#ifdef DEBUG
    auto debugSprite = std::make_shared<Sprite>(glm::vec<2, int>(2, 0));
    auto debugSpriteNode = std::make_shared<SpriteNode>(debugSprite, renderer);
    debugSpriteNode->GetLocalTransform()->SetScale({0.7f, 0.15f, 1.f});
    debugSpriteNode->GetLocalTransform()->SetPosition({0.f, 0.f, 10.f});
    jumpTrigger->AddChild(debugSpriteNode);
#endif

}

void PlayerNode::Update(struct MainEngine *engine, float seconds, float deltaSeconds) {
    glm::vec2 input = GetMovementInput(engine);

    glm::vec2 newAcceleration = GetAcceleration();

    if (std::abs(GetVelocity().x) < playerSpeed && std::abs(input.x) > 0 )
        newAcceleration.x = input.x * 100.f;
    else
        newAcceleration.x = GetVelocity().x * -10.f;

    auto jumpTrigger = dynamic_cast<RigidbodyNode*>(GetChild([](Node* node) -> bool {
        return dynamic_cast<RigidbodyNode*>(node) != nullptr;
    }));

    bool isGrounded = !jumpTrigger->GetOverlappedNodesThisFrame().empty();

    if (input.y > 0 && isGrounded){
        glm::vec2 newVelocity = GetVelocity();
        newVelocity.y = startJumpVelocity;
        SetVelocity(newVelocity);
    }

    newAcceleration.y = gravityAcceleration;

    if (!isGrounded && GetVelocity().y < 0.f) {
        newAcceleration.y *= fallGravityFactor;
    }

    if (input.y > 0.f)
    {
        newAcceleration.y *= buttonPressJumpGravityFactor;
    }

    SetAcceleration(newAcceleration);
    glm::vec2 velocity = GetVelocity();

    if (velocity.x > 0)
    {
        playerSprite->GetLocalTransform()->SetRotation({{0.f, 0.f, 0.f}});
    } else {
        playerSprite->GetLocalTransform()->SetRotation({{0.f, -glm::pi<float>(), 0.f}});
    }


    RigidbodyNode::Update(engine, seconds, deltaSeconds);
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

void PlayerNode::SetJumpParameters(float targetHeight, float targetDistance) {
    float jumpTime = (targetDistance / 4.f) / playerSpeed;
    jumpTime += (targetDistance / 4.f) / playerSpeed * fallGravityFactor;
    startJumpVelocity = 2 * targetHeight / jumpTime;
    gravityAcceleration = (-startJumpVelocity / jumpTime) / buttonPressJumpGravityFactor;
}

float PlayerNode::GetGravityAcceleration() const {
    return gravityAcceleration;
}

float PlayerNode::GetStartJumpVelocity() const {
    return startJumpVelocity;
}

float PlayerNode::GetFallGravityFactor() const {
    return fallGravityFactor;
}

void PlayerNode::SetPlayerSpeed(float playerSpeed) {
    PlayerNode::playerSpeed = playerSpeed;
}

void PlayerNode::SetFallGravityFactor(float fallGravityFactor) {
    PlayerNode::fallGravityFactor = fallGravityFactor;
}

void PlayerNode::SetButtonPressJumpGravityFactor(float buttonPressJumpGravityFactor) {
    PlayerNode::buttonPressJumpGravityFactor = buttonPressJumpGravityFactor;
}

std::shared_ptr<SpriteArrayNode> PlayerNode::CreatePlayerSprite(MainEngine* engine, SpriteRenderer* renderer) {
    std::vector<std::shared_ptr<Sprite>> playerSpriteArray = {
            std::make_shared<Sprite>(glm::ivec2(0, 2)),
            std::make_shared<Sprite>(glm::ivec2(1, 2)),
            std::make_shared<Sprite>(glm::ivec2(2, 2)),
            std::make_shared<Sprite>(glm::ivec2(3, 2))
    };
    std::vector<int> animation = {0, 1, 2, 3};

    auto result = std::make_shared<SpriteArrayNode>(playerSpriteArray, renderer);
    result->PlayAnimation(animation, 0.166f);

    return result;
}

