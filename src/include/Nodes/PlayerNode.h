#pragma once

#include "RigidbodyNode.h"

class PlayerNode: public RigidbodyNode{
private:
    float gravityAcceleration = -5.f;
public:
    PlayerNode();

    void Update(struct MainEngine *engine, float seconds, float deltaSeconds) override;
private:
    glm::vec2 GetMovementInput(MainEngine* Engine);
};
