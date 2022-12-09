#pragma once


#include <memory>
#include <glm/glm.hpp>
#include <glad/glad.h>

class Camera {
private:
    glm::vec3 Position;

    GLuint UboTransformMatrices;

    glm::vec<2, int> Resolution;
public:
    Camera();

    ~Camera();

    void SetPosition(glm::vec3 NewPosition);

    [[nodiscard]] glm::mat4 GetCameraProjectionMatrix(glm::vec<2, int> resolution) const;

    void UpdateProjection(glm::vec<2, int> resolution);
private:
    void UpdateView();
};
