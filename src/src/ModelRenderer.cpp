#include "ModelRenderer.h"

#include "Nodes/ModelNode.h"
#include "Model.h"
#include "LoggingMacros.h"

void ModelRenderer::Draw()
{
    for (auto& [Model, NodeArray] : NodesMap)
    {
        DrawModel(Model, NodeArray);
    }
}

void ModelRenderer::DrawModel(Model* Model, std::set<struct ModelNode*>& NodeArray)
{
    for (auto& Node: NodeArray)
    {
        if (Node->WasDirtyThisFrame())
        {
            UpdateMatrixBuffer(Model, NodeArray);
            break;
        }
    }

//    Model->GetShader()->Activate();
//
//    for (const auto& Mesh : Model->GetMeshes())
//    {
//        Mesh->BindTextures(*Model->GetShader());
//
//        glBindVertexArray(Mesh->GetVao().GetVaoId());
//        glDrawElementsInstanced(GL_TRIANGLES, Mesh->GetVao().GetIndicesCount(), GL_UNSIGNED_INT, 0, NodeArray.size());
//        glBindVertexArray(0);
//    }
}

void ModelRenderer::UpdateMatrixBuffer(Model* Model, std::set<ModelNode*>& NodeArray)
{
    std::vector<glm::mat4> Matrices;
    for (ModelNode* Node : NodeArray)
    {
        Matrices.push_back(*Node->GetWorldTransformMatrix());
    }

    GLuint Buffer = BufferMap[Model];
    glBindBuffer(GL_ARRAY_BUFFER, Buffer);
    glBufferData(GL_ARRAY_BUFFER, Matrices.size() * sizeof(glm::mat4), &Matrices[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ModelRenderer::AddNode(ModelNode* Node)
{
    NodesMap[Node->GetModel()].insert(Node);
    if (BufferMap.find(Node->GetModel()) == BufferMap.end())
    {
        unsigned int MatrixBuffer;
        glGenBuffers(1, &MatrixBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, MatrixBuffer);

        BufferMap[Node->GetModel()] = MatrixBuffer;

//        for (const auto& Mesh : Node->GetModel()->GetMeshes())
//        {
//            glBindVertexArray(Mesh->GetVao().GetVaoId());
//            GLsizei SizeOfVec4 = sizeof(glm::vec4);
//            glEnableVertexAttribArray(3);
//            glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * SizeOfVec4, (void*)0);
//            glEnableVertexAttribArray(4);
//            glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * SizeOfVec4, (void*)(SizeOfVec4));
//            glEnableVertexAttribArray(5);
//            glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * SizeOfVec4, (void*)(2 * SizeOfVec4));
//            glEnableVertexAttribArray(6);
//            glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * SizeOfVec4, (void*)(3 * SizeOfVec4));
//
//            glVertexAttribDivisor(3, 1);
//            glVertexAttribDivisor(4, 1);
//            glVertexAttribDivisor(5, 1);
//            glVertexAttribDivisor(6, 1);
//
//            glBindVertexArray(0);
//        }
    }
}

void ModelRenderer::RemoveNode(ModelNode* Node)
{
    auto NodesSet = NodesMap[Node->GetModel()];
    NodesSet.erase(Node);
    if (NodesSet.empty())
    {
        NodesMap.erase(Node->GetModel());
        GLuint Buffer = BufferMap[Node->GetModel()];
        glDeleteBuffers(1, &Buffer);
        BufferMap.erase(Node->GetModel());
    }
}
