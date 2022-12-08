#pragma once

#include <memory>
#include <vector>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

struct Vertex
{
    glm::vec3 Position;
};

struct Texture
{
    GLuint Id;
    std::string TexturePath;
};


class VAOWrapper
{
private:
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    GLsizei IndicesCount;
public:
    VAOWrapper(const std::vector<Vertex>& Vertices, const std::vector<GLuint>& Indices,
               const std::vector<Texture>& Textures);
    ~VAOWrapper();

    void Draw() const;

    GLuint GetVaoId() const;
    GLsizei GetIndicesCount() const;
};
