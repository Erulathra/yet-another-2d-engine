#include "SpriteRenderer.h"

#include <vector>
#include <filesystem>
#include <stb_image.h>

#include "Nodes/SpriteNode.h"
#include "VAOWrapper.h"
#include "Sprite.h"
#include "ShaderWrapper.h"

#include "LoggingMacros.h"


SpriteRenderer::SpriteRenderer(std::string tileMapPath, int tileSize)
        : tileSize(tileSize) {


    InitializeVAO();
    shader = std::make_unique<ShaderWrapper>("res/shaders/tile_map.vert", "res/shaders/tile_map.frag");
    tileMap = TextureFromFile(tileMapPath);

    glBindVertexArray(0);
}

GLuint SpriteRenderer::TextureFromFile(const std::string &path) {
    GLuint textureId;
    glGenTextures(1, &textureId);

    SPDLOG_DEBUG("Loading texture at path: {}", path);

    int width, height, NumberOfComponents;
    auto imageData = stbi_load(path.c_str(), &width, &height, &NumberOfComponents, 0);
    if (imageData) {
        GLenum colorFormat;
        if (NumberOfComponents == 1)
            colorFormat = (GL_RED);
        else if (NumberOfComponents == 3)
            colorFormat = (GL_RGB);
        else if (NumberOfComponents == 4)
            colorFormat = (GL_RGBA);

        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, colorFormat, width, height, 0, colorFormat, GL_UNSIGNED_BYTE, imageData);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        tileMapSize = width;

        stbi_image_free(imageData);
    } else {
        SPDLOG_ERROR("Failed to load texture at path: {}", path);
        stbi_image_free(imageData);
    }

    return textureId;
}

void SpriteRenderer::InitializeVAO() {
    std::vector<Vertex> vertices = {
            {glm::vec3(0.5f, 0.5f, 0.f)},
            {glm::vec3(0.5f, -0.5f, 0.f)},
            {glm::vec3(-0.5f, -0.5f, 0.f)},
            {glm::vec3(-0.5f, 0.5f, 0.f)},
    };

    std::vector<GLuint> indices = {0, 1, 3, 1, 2, 3};

    tileVAO = std::make_unique<VAOWrapper>(vertices, indices);

    glGenBuffers(1, &matrixBuffer);
    glGenBuffers(1, &textureCordBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, matrixBuffer);
    glBindVertexArray(tileVAO->GetVaoId());

    const GLsizei SizeOfVec4 = sizeof(glm::vec4);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * SizeOfVec4, (void *) 0);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * SizeOfVec4, (void *) (SizeOfVec4));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * SizeOfVec4, (void *) (2 * SizeOfVec4));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * SizeOfVec4, (void *) (3 * SizeOfVec4));

    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);

    glBindBuffer(GL_ARRAY_BUFFER, textureCordBuffer);

    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(5, 2, GL_INT, sizeof(glm::vec<2, int>), (void *) 0);
    glVertexAttribDivisor(5, 1);
}

void SpriteRenderer::AddNode(SpriteNode *node) {
    nodes.push_back(node);
}

void SpriteRenderer::RemoveNode(SpriteNode *node) {
    auto foundIterator = std::find(nodes.begin(), nodes.end(),node);
    nodes.erase(foundIterator);
}

void SpriteRenderer::UpdateMatrixBuffer() {
    std::vector<glm::mat4> matrices;
    for (SpriteNode *node: nodes) {
        matrices.push_back(*node->GetWorldTransformMatrix());
    }

    glBindBuffer(GL_ARRAY_BUFFER, matrixBuffer);
    glBufferData(GL_ARRAY_BUFFER, matrices.size() * sizeof(glm::mat4), &matrices[0], GL_DYNAMIC_DRAW);
}

void SpriteRenderer::UpdateTilePositionBuffer() {
    std::vector<glm::vec<2, int>> tileCoords;
    for (SpriteNode *node: nodes) {
        tileCoords.push_back(node->getSprite()->GetTileMapPosition());
    }

    glBindBuffer(GL_ARRAY_BUFFER, textureCordBuffer);
    glBufferData(GL_ARRAY_BUFFER, tileCoords.size() * sizeof(glm::vec<2, int>), &tileCoords[0], GL_DYNAMIC_DRAW);
}

void SpriteRenderer::Draw() {
    auto comparator = [](Node *A, Node *B)-> bool {
        glm::mat4 matrixA = *A->GetWorldTransformMatrix();
        glm::mat4 matrixB = *B->GetWorldTransformMatrix();

        if (matrixA[3][2] == matrixB[3][2])
            return A < B;
        else
            return matrixA[3][2] < matrixB[3][2];
    };

    for (SpriteNode *node: nodes) {
        if (node->WasDirtyThisFrame()) {
            std::sort(nodes.begin(), nodes.end(), comparator);
            UpdateMatrixBuffer();
            break;
        }
    }

    UpdateTilePositionBuffer();

    shader->Activate();
    shader->SetInt("tileSize", tileSize);
    shader->SetInt("tileMapSize", tileMapSize);

    glActiveTexture(GL_TEXTURE0);
    shader->SetFloat("texture_diffuse", 0);
    glBindTexture(GL_TEXTURE_2D, tileMap);

    glBindVertexArray(tileVAO->GetVaoId());
    glDrawElementsInstanced(GL_TRIANGLES, tileVAO->GetIndicesCount(), GL_UNSIGNED_INT, 0, nodes.size());
}

SpriteRenderer::~SpriteRenderer() {
    glDeleteBuffers(1, &matrixBuffer);
    glDeleteBuffers(1, &textureCordBuffer);
}
