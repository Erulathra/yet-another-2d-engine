#pragma once

#include <memory>

class CollisionShapeFactory {
private:
    std::shared_ptr<class CollisionShape> product;

    CollisionShapeFactory();
    explicit CollisionShapeFactory(CollisionShapeFactory* obj);
public:
    static std::shared_ptr<CollisionShapeFactory> CreateFactory();

    std::shared_ptr<CollisionShapeFactory> CreateRectangleCollisionShape(float height, float width);
    std::shared_ptr<CollisionShapeFactory> CreateCircleCollisionShape(float radius);

    std::shared_ptr<CollisionShape> Build();
};
