#include "Nodes/CollisionShapes/CircleCollisionShape.h"

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
