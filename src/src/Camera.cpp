#include "Camera.h"
#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

#include "LoggingMacros.h"

Camera::Camera()
        : Position(0.f, 0.f, 50.f) {
    UboTransformMatrices = 0;
    glGenBuffers(1, &UboTransformMatrices);
    glBindBuffer(GL_UNIFORM_BUFFER, UboTransformMatrices);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, UboTransformMatrices);
    UpdateView();
}

Camera::~Camera() {
    glDeleteBuffers(1, &UboTransformMatrices);
}

glm::mat4 Camera::GetCameraProjectionMatrix(glm::vec<2, int> resolution) const {
    float width = resolution.x / 32.;
    float height = resolution.y / 32.;

    return glm::ortho<float>(-width / 2, width / 2, -height / 2, height / 2, 0.1f, 100.f);
}

void Camera::UpdateProjection(glm::vec<2, int> resolution) {
    glBindBuffer(GL_UNIFORM_BUFFER, UboTransformMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(GetCameraProjectionMatrix(resolution)));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Camera::UpdateView() {
    glBindBuffer(GL_UNIFORM_BUFFER, UboTransformMatrices);
    glm::mat4 ViewMatrix = glm::lookAt(Position, Position + glm::vec3(0., 0., -1.f), glm::vec3(0.f, 1.f, 0.f));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(ViewMatrix));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Camera::SetPosition(glm::vec3 NewPosition) {
    Position = NewPosition;
    UpdateView();
}
