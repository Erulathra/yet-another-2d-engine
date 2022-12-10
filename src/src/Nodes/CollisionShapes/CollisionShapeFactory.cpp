#include "include/Nodes/CollisionShapes/CollisionShapeFactory.h"

#include "LoggingMacros.h"
#include "Nodes/CollisionShapes/RectangleCollisionShape.h"

std::shared_ptr<CollisionShapeFactory> CollisionShapeFactory::CreateFactory() {
    return std::shared_ptr<CollisionShapeFactory>(new CollisionShapeFactory());
}

CollisionShapeFactory::CollisionShapeFactory() {
    product = nullptr;
}

std::shared_ptr<CollisionShape> CollisionShapeFactory::Build() {
    if (product == nullptr)
        SPDLOG_ERROR("Empty Collision Shape Factory");

    return product->Clone();
}

std::shared_ptr<CollisionShapeFactory> CollisionShapeFactory::CreateRectangleCollisionShape(float height, float width) {
    std::shared_ptr<CollisionShapeFactory> result(new CollisionShapeFactory(this));
    result->product = std::make_shared<RectangleCollisionShape>(height, width);

    return result;
}

CollisionShapeFactory::CollisionShapeFactory(CollisionShapeFactory *obj) {
    CollisionShapeFactory result;
    result.product = obj->product;
}
