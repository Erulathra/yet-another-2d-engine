#pragma once

#include "RigidbodyNode.h"

class PlayerNode: public RigidbodyNode{
private:
    float gravityAcceleration;
    float startJumpVelocity;
    float playerSpeed;
public:
    PlayerNode(class MainEngine* engine, class SpriteRenderer* renderer);

    void Update(struct MainEngine *engine, float seconds, float deltaSeconds) override;

    void SetJumpParameters(float targetHeight, float targetDistance);

    float GetGravityAcceleration() const;
    float GetStartJumpVelocity() const;

private:
    glm::vec2 GetMovementInput(MainEngine* Engine);
};
