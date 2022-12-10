#include "Nodes/CollisionShapes/RectangleCollisionShape.h"
#include "Nodes/RigidbodyNode.h"

RectangleCollisionShape::RectangleCollisionShape(float height, float width) : height(height), width(width) {}

float RectangleCollisionShape::GetHeight() const {
    return height;
}

void RectangleCollisionShape::SetHeight(float height) {
    RectangleCollisionShape::height = height;
}

float RectangleCollisionShape::GetWidth() const {
    return width;
}

void RectangleCollisionShape::SetWidth(float width) {
    RectangleCollisionShape::width = width;
}

float RectangleCollisionShape::GetLeft(glm::vec3 position) const {
    return position.x - (width / 2);
}

float RectangleCollisionShape::GetRight(glm::vec3 position) const {
    return position.x + (width / 2);
}

float RectangleCollisionShape::GetTop(glm::vec3 position) const {
    return position.y + (height / 2);
}

float RectangleCollisionShape::GetBottom(glm::vec3 position) const {
    return position.y - (height / 2);
}

std::shared_ptr<CollisionShape> RectangleCollisionShape::Clone() {
    return std::make_shared<RectangleCollisionShape>(*this);
}

bool RectangleCollisionShape::IsRectanglesColliding(RigidbodyNode* selfNode, RigidbodyNode* anotherNode) {
    auto thisCollisionShape = dynamic_cast<RectangleCollisionShape *>(selfNode->GetCollisionShape().get());
    auto anotherCollisionShape = dynamic_cast<RectangleCollisionShape *>(anotherNode->GetCollisionShape().get());

    glm::vec3 thisPosition = selfNode->GetWorldPosition();
    glm::vec3 anotherPosition = anotherNode->GetWorldPosition();

    return thisCollisionShape->GetRight(thisPosition) > anotherCollisionShape->GetLeft(anotherPosition)
           && thisCollisionShape->GetLeft(thisPosition) < anotherCollisionShape->GetRight(anotherPosition)
           && thisCollisionShape->GetTop(thisPosition) > anotherCollisionShape->GetBottom(anotherPosition)
           && thisCollisionShape->GetBottom(thisPosition) < anotherCollisionShape->GetTop(anotherPosition);
}

glm::vec2
RectangleCollisionShape::GetSeparationVectorBetweenRectangles(RigidbodyNode* selfNode, RigidbodyNode* anotherNode) {
    auto thisCollisionShape = dynamic_cast<RectangleCollisionShape *>(selfNode->GetCollisionShape().get());
    auto anotherCollisionShape = dynamic_cast<RectangleCollisionShape *>(anotherNode->GetCollisionShape().get());

    glm::vec3 thisPosition = selfNode->GetWorldPosition();
    glm::vec3 anotherPosition = anotherNode->GetWorldPosition();

    float leftSeparation = thisCollisionShape->GetRight(thisPosition) - anotherCollisionShape->GetLeft(anotherPosition);
    float rightSeparation = anotherCollisionShape->GetRight(anotherPosition) - thisCollisionShape->GetLeft(thisPosition);
    float topSeparation = thisCollisionShape->GetTop(thisPosition) - anotherCollisionShape->GetBottom(anotherPosition);
    float bottomSeparation = anotherCollisionShape->GetTop(anotherPosition) - thisCollisionShape->GetBottom(thisPosition);

    glm::vec2 finalSeparation;
    finalSeparation.x = leftSeparation < rightSeparation ? -leftSeparation : rightSeparation;
    finalSeparation.y = bottomSeparation < topSeparation ? bottomSeparation : -topSeparation;

    if (std::abs(finalSeparation.x) < std::abs(finalSeparation.y))
        finalSeparation.y = 0.f;
    else
        finalSeparation.x = 0.f;

    return finalSeparation;
}
