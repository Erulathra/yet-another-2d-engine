#include "Nodes/CollisionShapes/CircleCollisionShape.h"
#include "Nodes/CollisionShapes/RectangleCollisionShape.h"
#include "Nodes/RigidbodyNode.h"

std::shared_ptr<CollisionShape> CircleCollisionShape::Clone() {
    return std::shared_ptr<CircleCollisionShape>(new CircleCollisionShape(this));
}

CircleCollisionShape::CircleCollisionShape(CircleCollisionShape* circleCollisionShape)
        : CircleCollisionShape(circleCollisionShape->radius) {
}

CircleCollisionShape::CircleCollisionShape(float radius) : radius(radius) {}

float CircleCollisionShape::GetRadius() const {
    return radius;
}

bool CircleCollisionShape::IsCirclesColliding(struct RigidbodyNode* selfNode, RigidbodyNode* anotherNode) {
    auto thisCollisionShape = dynamic_cast<CircleCollisionShape*>(selfNode->GetCollisionShape().get());
    auto anotherCollisionShape = dynamic_cast<CircleCollisionShape*>(anotherNode->GetCollisionShape().get());

    glm::vec2 thisPosition = glm::vec2(selfNode->GetWorldPosition());
    glm::vec2 anotherPosition = glm::vec2(anotherNode->GetWorldPosition());

    return glm::length(thisPosition - anotherPosition) <= thisCollisionShape->radius + anotherCollisionShape->radius;
}

glm::vec2 CircleCollisionShape::GetSeparationVectorBetweenCircles(RigidbodyNode* selfNode, RigidbodyNode* anotherNode) {
    auto thisCollisionShape = dynamic_cast<CircleCollisionShape*>(selfNode->GetCollisionShape().get());
    auto anotherCollisionShape = dynamic_cast<CircleCollisionShape*>(anotherNode->GetCollisionShape().get());

    glm::vec2 thisPosition = glm::vec2(selfNode->GetWorldPosition());
    glm::vec2 anotherPosition = glm::vec2(anotherNode->GetWorldPosition());

    glm::vec2 thisToAnotherPosition = thisPosition - anotherPosition;

   return glm::normalize(thisToAnotherPosition) *
           (thisCollisionShape->radius + anotherCollisionShape->radius - glm::length(thisToAnotherPosition));
}

bool CircleCollisionShape::IsCircleCollidingWithRectangle(RigidbodyNode* selfNode, RigidbodyNode* anotherNode) {
    auto thisCollisionShape = dynamic_cast<CircleCollisionShape*>(selfNode->GetCollisionShape().get());
    auto anotherCollisionShape = dynamic_cast<RectangleCollisionShape*>(anotherNode->GetCollisionShape().get());

    glm::vec2 thisPosition = glm::vec2(selfNode->GetWorldPosition());
    glm::vec2 anotherPosition = glm::vec2(anotherNode->GetWorldPosition());

    glm::vec2 nearestPoint = CalculateNearestPoint(anotherCollisionShape, thisPosition, anotherPosition);
    return glm::length(thisPosition - nearestPoint) <= thisCollisionShape->radius;
}

glm::vec2 CircleCollisionShape::CalculateNearestPoint(const RectangleCollisionShape* anotherCollisionShape,
                                                       const glm::vec2& thisPosition,
                                                       const glm::vec2& anotherPosition) {
    glm::vec2 nearestPoint;
    nearestPoint.x = glm::clamp(thisPosition.x, anotherCollisionShape->GetLeft(glm::vec3(anotherPosition, 0.f)),
                                anotherCollisionShape->GetRight(glm::vec3(anotherPosition, 0.f)));
    nearestPoint.y = glm::clamp(thisPosition.y, anotherCollisionShape->GetBottom(glm::vec3(anotherPosition, 0.f)),
                                anotherCollisionShape->GetTop(glm::vec3(anotherPosition, 0.f)));
    return nearestPoint;
}

glm::vec2 CircleCollisionShape::GetSeparationVectorBetweenCircleAndRectangle(RigidbodyNode* selfNode, RigidbodyNode* anotherNode) {
    auto thisCollisionShape = dynamic_cast<CircleCollisionShape*>(selfNode->GetCollisionShape().get());
    auto anotherCollisionShape = dynamic_cast<RectangleCollisionShape*>(anotherNode->GetCollisionShape().get());

    glm::vec2 thisPosition = glm::vec2(selfNode->GetWorldPosition());
    glm::vec2 anotherPosition = glm::vec2(anotherNode->GetWorldPosition());

    glm::vec2 nearestPoint = CalculateNearestPoint(anotherCollisionShape, thisPosition, anotherPosition);

    if (thisPosition == nearestPoint)
    {
        auto temporaryRectangle = std::make_shared<RectangleCollisionShape>(thisCollisionShape->radius * 2, thisCollisionShape->radius * 2);
        auto temporaryRigidbody = std::make_unique<RigidbodyNode>(temporaryRectangle);
        temporaryRigidbody->GetLocalTransform()->SetPosition({thisPosition, 0.f});
        temporaryRigidbody->CalculateWorldTransform();

        return RectangleCollisionShape::GetSeparationVectorBetweenRectangles(temporaryRigidbody.get(), anotherNode);
    }

    auto temporaryCircle = std::make_shared<CircleCollisionShape>(0);
    auto temporaryRigidbody = std::make_unique<RigidbodyNode>(temporaryCircle);
    temporaryRigidbody->GetLocalTransform()->SetPosition({nearestPoint, 0.f});
    temporaryRigidbody->CalculateWorldTransform();

    return CircleCollisionShape::GetSeparationVectorBetweenCircles(selfNode, temporaryRigidbody.get());

}
