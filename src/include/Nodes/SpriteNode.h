#pragma once

#include <memory>

#include "Node.h"

class SpriteNode: public Node {
private:

    class SpriteRenderer* renderer;

    explicit SpriteNode(const Node &obj);

protected:
    std::shared_ptr<class Sprite> sprite;

public:
    explicit SpriteNode(const std::shared_ptr<Sprite> &sprite, SpriteRenderer* renderer);

    std::shared_ptr<Node> Clone() const override;

    [[nodiscard]] const Sprite* getSprite() const;
    virtual ~SpriteNode();

protected:
    void Draw(glm::mat4 &ParentTransform, bool IsDirty) override;

};
