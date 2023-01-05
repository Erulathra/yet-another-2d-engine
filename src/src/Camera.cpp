#include "Camera.h"
#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

#include "LoggingMacros.h"

Camera::Camera()
        : position(0.f, 0.f, 50.f), scale(32.f) {
    uboTransformMatrices = 0;
    glGenBuffers(1, &uboTransformMatrices);
    glBindBuffer(GL_UNIFORM_BUFFER, uboTransformMatrices);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboTransformMatrices);
    UpdateView();
}

Camera::~Camera() {
    glDeleteBuffers(1, &uboTransformMatrices);
}

glm::mat4 Camera::GetCameraProjectionMatrix(glm::vec<2, int> resolution) const {
    float width = resolution.x / scale;
    float height = resolution.y / scale;

    return glm::ortho<float>(-width / 2, width / 2, -height / 2, height / 2, 0.1f, 100.f);
}

void Camera::UpdateProjection(glm::vec<2, int> resolution) {
    glBindBuffer(GL_UNIFORM_BUFFER, uboTransformMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(GetCameraProjectionMatrix(resolution)));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Camera::UpdateView() {
    glBindBuffer(GL_UNIFORM_BUFFER, uboTransformMatrices);
    glm::mat4 viewMatrix = glm::lookAt(position, position + glm::vec3(0., 0., -1.f), glm::vec3(0.f, 1.f, 0.f));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(viewMatrix));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Camera::SetPosition(glm::vec3 newPosition) {
    if (position != newPosition) {
        position = newPosition;
        UpdateView();
    }
}

float Camera::GetScale() const {
    return scale;
}

void Camera::SetScale(float scale) {
    Camera::scale = scale;
}
