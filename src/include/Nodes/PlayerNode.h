#pragma once


#include "Node.h"

class PlayerNode: public Node{
public:
    PlayerNode(const std::shared_ptr<class SpriteNode>& sprite);

    void Update(struct MainEngine *engine, float seconds, float deltaSeconds) override;
private:
    glm::vec2 GetMovementInput(MainEngine* Engine);
};
