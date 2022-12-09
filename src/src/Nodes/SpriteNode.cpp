#include "Nodes/SpriteNode.h"
#include "Sprite.h"
#include "SpriteRenderer.h"

SpriteNode::SpriteNode(const std::shared_ptr<Sprite> &sprite, SpriteRenderer* renderer)
        :sprite(sprite), renderer(renderer) {
    renderer->AddNode(this);
}

SpriteNode::~SpriteNode() {
    renderer->RemoveNode(this);
}

void SpriteNode::Draw(glm::mat4 &ParentTransform, bool IsDirty) {
    Node::Draw(ParentTransform, IsDirty);
}

const Sprite *SpriteNode::getSprite() const {
    return static_cast<const Sprite *>(sprite.get());
}

