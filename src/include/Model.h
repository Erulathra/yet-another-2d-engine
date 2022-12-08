#pragma once

#include "Mesh.h"

class Model {
private:
    std::shared_ptr<ShaderWrapper> Shader;
    std::vector<std::shared_ptr<Mesh>> Meshes;
    std::string ModelPath;

public:
    explicit Model(const std::string &Path, std::shared_ptr<ShaderWrapper> Shared);

    void Draw();


private:
    GLuint TextureFromFile(const std::string &Path);
};
