#pragma once

#include <memory>
#include <set>
#include <glad/glad.h>

class SpriteRenderer {
private:
    std::unique_ptr<class VAOWrapper> tileVAO;
    std::unique_ptr<class ShaderWrapper> shader;
    std::set<class SpriteNode*> nodes;

    GLuint matrixBuffer, textureCordBuffer;

    GLuint tileMap;
    int tileSize;
    int tileMapSize;
public:
    SpriteRenderer(std::string tileMapPath, int tileSize);

    void Draw();

    void AddNode(SpriteNode* node);
    void RemoveNode(SpriteNode* node);

    virtual ~SpriteRenderer();
private:
    void UpdateMatrixBuffer();
    void UpdateTilePositionBuffer();

    void InitializeVAO();

    GLuint TextureFromFile(const std::string& path);
};
