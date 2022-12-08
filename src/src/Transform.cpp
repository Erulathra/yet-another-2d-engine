#include "Transform.h"

#include <glm/gtc/matrix_transform.hpp>

glm::mat4 Transform::GetMatrix() const {
    glm::mat4 Translation = glm::translate(glm::mat4(1.f), Position);
    glm::mat4 ScaleMat = glm::scale(glm::mat4(1.f), Scale);
    return Translation * Rotation * ScaleMat;
}

glm::vec3 Transform::GetPosition() const {
    return Position;
}

glm::mat4 Transform::GetRotation() const {
    return Rotation;
}

glm::vec3 Transform::GetScale() const {
    return Scale;
}

void Transform::SetPosition(const glm::vec3& NewPosition) {
    Position = NewPosition;
}

void Transform::SetRotation(const glm::mat4& NewRotation) {
    Rotation = NewRotation;
}

void Transform::SetScale(const glm::vec3& NewScale) {
    Scale = NewScale;
}

Transform::Transform() : Position(glm::vec3(0.f)), Rotation(glm::mat4(1.f)), Scale(glm::vec3(1.f)) {}

Transform::Transform(Transform* OriginalTransform) :
Position(OriginalTransform->Position),
Rotation(OriginalTransform->Rotation),
Scale(OriginalTransform->Scale) {
}
