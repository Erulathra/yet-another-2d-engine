#include "Nodes/CollisionShapes/RectangleCollisionShape.h"

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
