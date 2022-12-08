#include "VAOWrapper.h"


VAOWrapper::~VAOWrapper()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

VAOWrapper::VAOWrapper(const std::vector<Vertex>& Vertices, const std::vector<GLuint>& Indices,
                       const std::vector<Texture>& Textures)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLint>(Vertices.size() * sizeof(Vertex)),
                 &Vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLint>(Indices.size() * sizeof(GLuint)),
                 &Indices[0], GL_STATIC_DRAW);

    IndicesCount = Indices.size();

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);

    glBindVertexArray(0);
}

void VAOWrapper::Draw() const
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, IndicesCount, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

GLuint VAOWrapper::GetVaoId() const
{
    return VAO;
}

GLsizei VAOWrapper::GetIndicesCount() const
{
    return IndicesCount;
}

