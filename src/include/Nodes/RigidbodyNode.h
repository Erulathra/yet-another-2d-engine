#pragma once


#include "Node.h"
#include "MainEngine.h"
#include "eventpp/callbacklist.h"

class RigidbodyNode : public Node {
private:
    std::shared_ptr<class CollisionShape> collisionShape;

    glm::vec2 acceleration;
    glm::vec2 velocity;

    glm::vec2 newAcceleration;

    bool isKinematic;
    bool isTrigger;

    explicit RigidbodyNode(const Node& obj);

    std::vector<RigidbodyNode*> overlappedNodesThisFrame;

public:
    eventpp::CallbackList<void(RigidbodyNode*)> onCollisionEnter;

    explicit RigidbodyNode(std::shared_ptr<class CollisionShapeFactory> collisionShapeFactory);
    explicit RigidbodyNode(std::shared_ptr<class CollisionShape> collisionShape);

    void Update(struct MainEngine* engine, float seconds, float deltaSeconds) override;

    [[nodiscard]] std::shared_ptr<Node> Clone() const override;
    [[nodiscard]] const glm::vec2& GetVelocity() const;
    [[nodiscard]] const glm::vec2& GetAcceleration() const;
    [[nodiscard]] bool IsKinematic() const;
    [[nodiscard]] bool IsTrigger() const;
    [[nodiscard]] const std::shared_ptr<struct CollisionShape>& GetCollisionShape() const;
    std::vector<RigidbodyNode*> GetOverlappedNodesThisFrame();

    void SetVelocity(const glm::vec2& velocity);
    void SetAcceleration(const glm::vec2& acceleration);
    void SetIsKinematic(bool isKinematic);
    void SetIsTrigger(bool isTrigger);
    void SetCollisionShape(const std::shared_ptr<struct CollisionShape>& collisionShape);

protected:
    glm::vec2 CalculateSeparationVector(RigidbodyNode* selfRigidbody, RigidbodyNode* anotherRigidbody);

    void HandleCollisions(MainEngine* engine);
    void Collide(RigidbodyNode* anotherRigidbodyNode);
    void HandlePhysics(float deltaSeconds);
};
