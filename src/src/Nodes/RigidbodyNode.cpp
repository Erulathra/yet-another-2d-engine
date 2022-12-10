#include "Nodes/RigidbodyNode.h"
#include "Nodes/CollisionShapes/CollisionShapeFactory.h"
#include "Nodes/CollisionShapes/CollisionShape.h"

#include "MainEngine.h"
#include "LoggingMacros.h"
#include "Nodes/CollisionShapes/RectangleCollisionShape.h"
#include "Nodes/CollisionShapes/CircleCollisionShape.h"

RigidbodyNode::RigidbodyNode(std::shared_ptr<CollisionShapeFactory> collisionShapeFactory)
        : acceleration(0.f), velocity(0.f), isKinematic(false) {
    collisionShape = collisionShapeFactory->Build();
}

const glm::vec2& RigidbodyNode::GetAcceleration() const {
    return acceleration;
}

void RigidbodyNode::SetAcceleration(const glm::vec2& acceleration) {
    RigidbodyNode::acceleration = acceleration;
}

const glm::vec2& RigidbodyNode::GetVelocity() const {
    return velocity;
}

void RigidbodyNode::SetVelocity(const glm::vec2& velocity) {
    RigidbodyNode::velocity = velocity;
}

std::shared_ptr<Node> RigidbodyNode::Clone() const {
    std::shared_ptr<RigidbodyNode> result(new RigidbodyNode(*Node::Clone()));
    result->acceleration = acceleration;
    result->velocity = velocity;
    result->collisionShape = collisionShape->Clone();
    result->isKinematic = isKinematic;
    return result;
}

RigidbodyNode::RigidbodyNode(const Node& obj)
        : Node(obj), acceleration(0.f), velocity(0.f) {

}

void RigidbodyNode::Update(MainEngine* engine, float seconds, float deltaSeconds) {
    Node::Update(engine, seconds, deltaSeconds);

    if (isKinematic)
        return;

    std::vector<Node*> foundNodes;
    engine->GetSceneRoot().GetAllNodes(foundNodes, [this](Node* node) -> bool {
        auto* rigidbodyNode = dynamic_cast<RigidbodyNode*>(node);
        return rigidbodyNode != nullptr && rigidbodyNode != this;
    });

    for (auto node: foundNodes) {
        auto* rigidbodyNode = dynamic_cast<RigidbodyNode*>(node);
        Collide(rigidbodyNode);
    }
}

void RigidbodyNode::Collide(RigidbodyNode* anotherRigidbodyNode) {
    glm::vec2 separationVector = CalculateSeparationVector(this, anotherRigidbodyNode);

    if (glm::length(separationVector) <= 0)
        return;

    glm::vec3 position = GetLocalTransform()->GetPosition();

    if (anotherRigidbodyNode->isKinematic) {
        position += glm::vec3(separationVector, 0.f);
    } else {
        separationVector /= 2.f;
        position += glm::vec3(separationVector, 0.f);

        glm::vec3 anotherPosition = anotherRigidbodyNode->GetLocalTransform()->GetPosition();
        anotherPosition -= glm::vec3(separationVector, 0.f);
        anotherRigidbodyNode->GetLocalTransform()->SetPosition(anotherPosition);
    }

    GetLocalTransform()->SetPosition(position);
}

glm::vec2 RigidbodyNode::CalculateSeparationVector(RigidbodyNode* selfRigidbody, RigidbodyNode* anotherRigidbody) {
    CollisionShape* thisCollisionShape = this->collisionShape.get();
    CollisionShape* anotherCollisionShape = anotherRigidbody->collisionShape.get();

    auto thisRectangleCollision = dynamic_cast<RectangleCollisionShape*>(thisCollisionShape);
    auto anotherRectangleCollision = dynamic_cast<RectangleCollisionShape*>(anotherCollisionShape);

    auto thisCircleCollision = dynamic_cast<CircleCollisionShape*>(thisCollisionShape);
    auto anotherCircleCollision = dynamic_cast<CircleCollisionShape*>(anotherCollisionShape);

    if (thisRectangleCollision && anotherRectangleCollision) {
        if (RectangleCollisionShape::IsRectanglesColliding(this, anotherRigidbody))
            return RectangleCollisionShape::GetSeparationVectorBetweenRectangles(this, anotherRigidbody);
        else
            return glm::vec2(0.f);
    }

//    if (thisCircleCollision && anotherCircleCollision) {
//        if (CircleCollisionShape::IsCirclesColliding(this, anotherRigidbody))
//            return CircleCollisionShape::GetSeparationVectorBetweenCircles(this, anotherRigidbody);
//        else
//            return glm::vec2(0.f);
//    }

    return glm::vec2(0.f);
}

bool RigidbodyNode::IsKinematic() const {
    return isKinematic;
}

void RigidbodyNode::SetIsKinematic(bool isKinematic) {
    RigidbodyNode::isKinematic = isKinematic;
}

const std::shared_ptr<struct CollisionShape>& RigidbodyNode::GetCollisionShape() const {
    return collisionShape;
}

void RigidbodyNode::SetCollisionShape(const std::shared_ptr<struct CollisionShape>& collisionShape) {
    RigidbodyNode::collisionShape = collisionShape;
}

