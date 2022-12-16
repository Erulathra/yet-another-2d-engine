#pragma once

#include "RigidbodyNode.h"

class PlayerNode: public RigidbodyNode{
private:
    float gravityAcceleration;
    float startJumpVelocity;
public:
    PlayerNode(class MainEngine* engine, class SpriteRenderer* renderer);

    void Update(struct MainEngine *engine, float seconds, float deltaSeconds) override;

    void SetJumpParameters(float targetHeight, float timeToJumpApex);
private:
    glm::vec2 GetMovementInput(MainEngine* Engine);
};
