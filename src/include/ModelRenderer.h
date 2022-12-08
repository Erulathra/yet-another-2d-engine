#pragma once

#include <map>
#include <set>
#include "glad/glad.h"

class ModelRenderer
{
private:
    std::map<class Model*, std::set<class ModelNode*>> NodesMap;
    std::map<class Model*, GLuint> BufferMap;
public:
    void Draw();

    void AddNode(ModelNode* Node);
    void RemoveNode(ModelNode* Node);
    void DrawModel(Model* Model, std::set<struct ModelNode*>& NodeArray);
    void UpdateMatrixBuffer(Model* Model, std::set<struct ModelNode*>& NodeArray);
};
