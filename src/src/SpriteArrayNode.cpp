#include "Nodes/SpriteArrayNode.h"
#include "SpriteRenderer.h"

SpriteArrayNode::SpriteArrayNode(const std::vector<std::shared_ptr<Sprite>>& spriteArray, SpriteRenderer* renderer)
: SpriteNode(spriteArray[0], renderer), currentAnimation() {
    this->spriteArray = spriteArray;
    timeFromLastFrame = 0;
    timeBetweenFrames = 0;
    currentFrame = 0;
}

void SpriteArrayNode::PlayAnimation(const std::vector<int>& animation, float timeBetweenFrames, bool loop) {
    this->currentAnimation = animation;
    this->timeBetweenFrames = timeBetweenFrames;
    this->isLooping = loop;
    currentFrame = 0;
}

std::shared_ptr<Node> SpriteArrayNode::Clone() const {
    //TODO
    return nullptr;
}

void SpriteArrayNode::Update(struct MainEngine* engine, float seconds, float deltaSeconds) {
    timeFromLastFrame += deltaSeconds;

    if (timeFromLastFrame < timeBetweenFrames)
    {
        Node::Update(engine, seconds, deltaSeconds);
        return;
    }

    if (currentAnimation.empty())
    {
        Node::Update(engine, seconds, deltaSeconds);
        return;
    }


    if (currentFrame >= spriteArray.size()) {
        if (isLooping) {
            currentFrame = 0;
        } else {
            currentAnimation.clear();
        }
    }

    int spriteIndex = currentAnimation[currentFrame];
    sprite = spriteArray[spriteIndex];
    timeFromLastFrame = 0.f;
    currentFrame++;

    Node::Update(engine, seconds, deltaSeconds);
}
