#pragma once

#include <glm/glm.hpp>

class Transform {
private:
    glm::vec3 Position;
    glm::vec3 Scale;

//TODO: Change to quanternion
    glm::mat4 Rotation;

public:
    Transform();
    Transform(Transform* OriginalTransform);

    [[nodiscard]] glm::vec3 GetPosition() const;
    [[nodiscard]] glm::mat4 GetRotation() const;
    [[nodiscard]] glm::vec3 GetScale() const;

    void SetPosition(const glm::vec3& NewPosition);
    void SetRotation(const glm::mat4& NewRotation);
    void SetScale(const glm::vec3& NewScale);

    [[nodiscard]] glm::mat4 GetMatrix() const;
};
