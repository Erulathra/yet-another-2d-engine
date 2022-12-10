#pragma once


#include "CollisionShape.h"
#include "glm/glm.hpp"

class RectangleCollisionShape : public CollisionShape {
private:
    float height;
    float width;

public:
    RectangleCollisionShape(float height, float width);

     std::shared_ptr<CollisionShape> Clone() override;

    [[nodiscard]] float GetLeft(glm::vec3 position) const;
    [[nodiscard]] float GetRight(glm::vec3 position) const;
    [[nodiscard]] float GetTop(glm::vec3 position) const;
    [[nodiscard]] float GetBottom(glm::vec3 position) const;

    [[nodiscard]] float GetHeight() const;
    [[nodiscard]] float GetWidth() const;

    void SetHeight(float height);
    void SetWidth(float width);
};
