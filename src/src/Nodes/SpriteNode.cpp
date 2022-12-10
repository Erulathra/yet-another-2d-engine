#include "Nodes/SpriteNode.h"
#include "Sprite.h"
#include "SpriteRenderer.h"

SpriteNode::SpriteNode(const std::shared_ptr<Sprite> &sprite, SpriteRenderer* renderer)
        :Node(), sprite(sprite), renderer(renderer) {
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

std::shared_ptr<Node> SpriteNode::Clone() const {
    std::shared_ptr<SpriteNode> result(new SpriteNode(*Node::Clone()));

    result->sprite = this->sprite;
    result->renderer = this->renderer;
    result->renderer->AddNode(result.get());

    return result;
}

SpriteNode::SpriteNode(const Node &obj) : Node(obj) {
    sprite = nullptr;
    renderer = nullptr;
}


