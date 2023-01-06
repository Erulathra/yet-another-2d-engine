#pragma once

#include "RigidbodyNode.h"

class PlayerNode: public RigidbodyNode{
private:
    float gravityAcceleration;
    float startJumpVelocity;
    float playerSpeed;
    float fallGravityFactor;
    float buttonPressJumpGravityFactor;

    std::shared_ptr<Node> playerSprite;
public:
    PlayerNode(class MainEngine* engine, class SpriteRenderer* renderer);

    void Update(struct MainEngine *engine, float seconds, float deltaSeconds) override;

    void SetJumpParameters(float targetHeight, float targetDistance);
    void SetPlayerSpeed(float playerSpeed);
    void SetFallGravityFactor(float fallGravityFactor);
    void SetButtonPressJumpGravityFactor(float buttonPressJumpGravityFactor);

    float GetGravityAcceleration() const;
    float GetStartJumpVelocity() const;
    float GetFallGravityFactor() const;

private:
    glm::vec2 GetMovementInput(MainEngine* Engine);
    std::shared_ptr<class SpriteArrayNode> CreatePlayerSprite(MainEngine* engine, SpriteRenderer* renderer);
};
