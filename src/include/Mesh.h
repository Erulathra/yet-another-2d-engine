#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>

#include "VAOWrapper.h"
#include "ShaderWrapper.h"

class Mesh
{
private:
    std::vector<Vertex> Vertices;
    std::vector<GLuint> Indices;
    std::vector<Texture> Textures;

    VAOWrapper Vao;
public:
    Mesh(const std::vector<Vertex>& Vertices, const std::vector<GLuint>& Indices, const std::vector<Texture>& Textures);
    void Draw(ShaderWrapper& Shader) const;

    const VAOWrapper& GetVao() const;
    void BindTextures(const ShaderWrapper& Shader) const;
};
