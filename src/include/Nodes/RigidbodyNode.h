#pragma once


#include "Node.h"

class RigidbodyNode : public Node {
private:
    std::shared_ptr<class CollisionShape> collisionShape;

    glm::vec2 acceleration;
    glm::vec2 velocity;

    bool isKinematic;

    explicit RigidbodyNode(const Node& obj);

public:
    explicit RigidbodyNode(std::shared_ptr<class CollisionShapeFactory> collisionShapeFactory);

    void Update(struct MainEngine* engine, float seconds, float deltaSeconds) override;

    [[nodiscard]] std::shared_ptr<Node> Clone() const override;
    [[nodiscard]] const glm::vec2& GetVelocity() const;
    [[nodiscard]] const glm::vec2& GetAcceleration() const;
    [[nodiscard]] bool IsKinematic() const;
    [[nodiscard]] const std::shared_ptr<struct CollisionShape>& GetCollisionShape() const;

    void SetVelocity(const glm::vec2& velocity);
    void SetAcceleration(const glm::vec2& acceleration);
    void SetIsKinematic(bool isKinematic);
    void SetCollisionShape(const std::shared_ptr<struct CollisionShape>& collisionShape);

protected:
    RigidbodyNode();

private:
    bool IsCollidingWithRectangle(RigidbodyNode* anotherRigidbodyNode);

    glm::vec2 CalculateSeparationVectorWithRectangle(RigidbodyNode* anotherRigidbodyNode);

    void CollideWithRectangle(RigidbodyNode* rigidbodyNode);
};
