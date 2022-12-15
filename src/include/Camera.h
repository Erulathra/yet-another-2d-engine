#pragma once


#include <memory>
#include <glm/glm.hpp>
#include <glad/glad.h>

class Camera {
private:
    glm::vec3 position;

    GLuint uboTransformMatrices;

    float scale;
    glm::vec<2, int> Resolution;
public:
    Camera();

    ~Camera();

    void SetPosition(glm::vec3 newPosition);

    [[nodiscard]] glm::mat4 GetCameraProjectionMatrix(glm::vec<2, int> resolution) const;

    void UpdateProjection(glm::vec<2, int> resolution);

    [[nodiscard]] float GetScale() const;

    void SetScale(float scale);

private:
    void UpdateView();
};
