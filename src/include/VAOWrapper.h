#pragma once

#include <memory>
#include <vector>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 position;
};

struct Texture {
    GLuint id;
    std::string texturePath;
};


class VAOWrapper {
private:
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    GLsizei indicesCount;
public:
    VAOWrapper(const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices);

    ~VAOWrapper();

    void Draw() const;

    GLuint GetVaoId() const;

    GLsizei GetIndicesCount() const;
};
