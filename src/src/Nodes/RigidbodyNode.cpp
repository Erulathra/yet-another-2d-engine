#include "Nodes/RigidbodyNode.h"
#include "Nodes/CollisionShapes/CollisionShapeFactory.h"
#include "Nodes/CollisionShapes/CollisionShape.h"

#include "MainEngine.h"
#include "LoggingMacros.h"
#include "Nodes/CollisionShapes/RectangleCollisionShape.h"

RigidbodyNode::RigidbodyNode(std::shared_ptr<CollisionShapeFactory> collisionShapeFactory)
        : acceleration(0.f), velocity(0.f), isKinematic(false) {
    collisionShape = collisionShapeFactory->Build();
}

const glm::vec2 &RigidbodyNode::GetAcceleration() const {
    return acceleration;
}

void RigidbodyNode::SetAcceleration(const glm::vec2 &acceleration) {
    RigidbodyNode::acceleration = acceleration;
}

const glm::vec2 &RigidbodyNode::GetVelocity() const {
    return velocity;
}

void RigidbodyNode::SetVelocity(const glm::vec2 &velocity) {
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

RigidbodyNode::RigidbodyNode(const Node &obj)
        : Node(obj), acceleration(0.f), velocity(0.f) {

}

void RigidbodyNode::Update(MainEngine *engine, float seconds, float deltaSeconds) {
    Node::Update(engine, seconds, deltaSeconds);

    if (isKinematic)
        return;

    std::vector<Node *> foundNodes;
    engine->GetSceneRoot().GetAllNodes(foundNodes, [this](Node *node) -> bool {
        auto *rigidbodyNode = dynamic_cast<RigidbodyNode *>(node);
        return rigidbodyNode != nullptr && rigidbodyNode != this;
    });

    for (auto node: foundNodes) {
        auto *rigidbodyNode = dynamic_cast<RigidbodyNode *>(node);

        CollisionShape *rigidbodyCollisionShape = rigidbodyNode->collisionShape.get();

        if (dynamic_cast<RectangleCollisionShape *>(rigidbodyCollisionShape) &&
            dynamic_cast<RectangleCollisionShape *>(this->collisionShape.get())) {
            CollideWithRectangle(rigidbodyNode);
        }
    }
}

bool RigidbodyNode::IsKinematic() const {
    return isKinematic;
}

void RigidbodyNode::SetIsKinematic(bool isKinematic) {
    RigidbodyNode::isKinematic = isKinematic;
}

void RigidbodyNode::CollideWithRectangle(RigidbodyNode* rigidbodyNode) {
    if (!IsCollidingWithRectangle(rigidbodyNode))
        return;

    glm::vec2 separationVector = CalculateSeparationVectorWithRectangle(rigidbodyNode);
    glm::vec3 position = GetLocalTransform()->GetPosition();

    if (rigidbodyNode->isKinematic) {
        position += glm::vec3(separationVector, 0.f);
    }
    else {
        separationVector /= 2.f;
        position += glm::vec3(separationVector, 0.f);

        glm::vec3 anotherPosition = rigidbodyNode->GetLocalTransform()->GetPosition();
        anotherPosition -= glm::vec3(separationVector, 0.f);
        rigidbodyNode->GetLocalTransform()->SetPosition(anotherPosition);
    }

    GetLocalTransform()->SetPosition(position);
}

bool RigidbodyNode::IsCollidingWithRectangle(RigidbodyNode *anotherRigidbodyNode) {
    auto thisCollisionShape = dynamic_cast<RectangleCollisionShape *>(this->collisionShape.get());
    auto anotherCollisionShape = dynamic_cast<RectangleCollisionShape *>(this->collisionShape.get());

    glm::vec3 thisPosition = GetWorldPosition();
    glm::vec3 anotherPosition = anotherRigidbodyNode->GetWorldPosition();

    return thisCollisionShape->GetRight(thisPosition) > anotherCollisionShape->GetLeft(anotherPosition)
           && thisCollisionShape->GetLeft(thisPosition) < anotherCollisionShape->GetRight(anotherPosition)
           && thisCollisionShape->GetTop(thisPosition) > anotherCollisionShape->GetBottom(anotherPosition)
           && thisCollisionShape->GetBottom(thisPosition) < anotherCollisionShape->GetTop(anotherPosition);
}

glm::vec2 RigidbodyNode::CalculateSeparationVectorWithRectangle(RigidbodyNode* anotherRigidbodyNode) {
    auto thisCollisionShape = dynamic_cast<RectangleCollisionShape *>(this->collisionShape.get());
    auto anotherCollisionShape = dynamic_cast<RectangleCollisionShape *>(this->collisionShape.get());

    glm::vec3 thisPosition = GetWorldPosition();
    glm::vec3 anotherPosition = anotherRigidbodyNode->GetWorldPosition();

    float leftSeparation = thisCollisionShape->GetRight(thisPosition) - anotherCollisionShape->GetLeft(anotherPosition);
    float rightSeparation = anotherCollisionShape->GetRight(anotherPosition) - thisCollisionShape->GetLeft(thisPosition);
    float topSeparation = thisCollisionShape->GetTop(thisPosition) - anotherCollisionShape->GetBottom(anotherPosition);
    float bottomSeparation = anotherCollisionShape->GetTop(anotherPosition) - thisCollisionShape->GetBottom(thisPosition);

    glm::vec2 finalSeparation;
    if (leftSeparation < rightSeparation)
        finalSeparation.x = -leftSeparation;
    else
        finalSeparation.x = rightSeparation;

    if (bottomSeparation < topSeparation)
        finalSeparation.y = bottomSeparation;
    else
        finalSeparation.y = -topSeparation;

    if (std::abs(finalSeparation.x) < std::abs(finalSeparation.y))
        finalSeparation.y = 0.f;
    else
        finalSeparation.x = 0.f;

    return finalSeparation;
}

RigidbodyNode::RigidbodyNode()
:velocity(0.f), acceleration(0.f), isKinematic(false) {

}

const std::shared_ptr<struct CollisionShape>& RigidbodyNode::GetCollisionShape() const {
    return collisionShape;
}

void RigidbodyNode::SetCollisionShape(const std::shared_ptr<struct CollisionShape>& collisionShape) {
    RigidbodyNode::collisionShape = collisionShape;
}


