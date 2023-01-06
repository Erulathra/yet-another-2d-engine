#pragma once

#include "SpriteNode.h"

class SpriteArrayNode: public SpriteNode {
private:
    std::vector<std::shared_ptr<Sprite>> spriteArray;
    std::vector<int> currentAnimation;

    int currentFrame;
    float timeFromLastFrame;
    float timeBetweenFrames;
    bool isLooping;
public:
    SpriteArrayNode(const std::vector<std::shared_ptr<Sprite>>& spriteArray, class SpriteRenderer* renderer);
    void PlayAnimation(const std::vector<int>& animation, float timeBetweenFrames, bool loop = true);

private:
    void Update(struct MainEngine* engine, float seconds, float deltaSeconds) override;

private:
    [[nodiscard]] std::shared_ptr<Node> Clone() const override;
};
