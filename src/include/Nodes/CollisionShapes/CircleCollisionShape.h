#pragma once


#include "CollisionShape.h"
#include "glm/vec2.hpp"
#include "RectangleCollisionShape.h"

class CircleCollisionShape : public CollisionShape {
private:
    float radius;

    explicit CircleCollisionShape(CircleCollisionShape* circleCollisionShape);
public:
    explicit CircleCollisionShape(float radius);

    static bool IsCirclesColliding(class RigidbodyNode* selfNode, RigidbodyNode* anotherNode);
    static glm::vec2 GetSeparationVectorBetweenCircles(RigidbodyNode* selfNode, RigidbodyNode* anotherNode);

    static bool IsCircleCollidingWithRectangle(RigidbodyNode* selfNode, RigidbodyNode* anotherNode);
    static glm::vec2 GetSeparationVectorBetweenCircleAndRectangle(RigidbodyNode* selfNode, RigidbodyNode* anotherNode);

    float GetRadius() const;

    std::shared_ptr<CollisionShape> Clone() override;

    static glm::vec2 CalculateNearestPoint(const RectangleCollisionShape* anotherCollisionShape,
                                           const glm::vec2& thisPosition, const glm::vec2& anotherPosition);
};
