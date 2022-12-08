#include "Nodes/Node.h"
#include "LoggingMacros.h"

Node::Node()
: LocalTransformPtr(std::make_shared<Transform>()), IsDirty(true), WorldTransformMatrix(1.f)
{

}

Transform* Node::GetLocalTransform()
{
    IsDirty = true;
    return LocalTransformPtr.get();
}

const glm::mat4* Node::GetWorldTransformMatrix()
{
    return &WorldTransformMatrix;
}

void Node::Draw()
{
    glm::mat4 WorldMatrix = *GetWorldTransformMatrix();
    Draw(WorldMatrix, IsDirty);
}

void Node::CalculateWorldTransform()
{
    glm::mat4 LocalTransformMatrix = LocalTransformPtr->GetMatrix();
    CalculateWorldTransform(LocalTransformMatrix, IsDirty);
}

void Node::Draw(glm::mat4& ParentTransform, bool IsDirty)
{
    for (const std::shared_ptr<Node>& Child: ChildrenList)
    {
        Child->Draw(WorldTransformMatrix, IsDirty);
    }
}

void Node::CalculateWorldTransform(glm::mat4& ParentTransform, bool IsDirty)
{
    IsDirty |= this->IsDirty;
    WasDirty = IsDirty;
    if (IsDirty)
    {
        WorldTransformMatrix = ParentTransform * LocalTransformPtr->GetMatrix();
        this->IsDirty = false;
    }


    for (const std::shared_ptr<Node>& Child: ChildrenList)
    {
        Child->CalculateWorldTransform(WorldTransformMatrix, IsDirty);
    }
}

void Node::AddChild(std::shared_ptr<Node> NewChild)
{
    if (NewChild.get() == this || NewChild.get() == Parent)
        return;

    NewChild->Parent = NewChild.get();
    ChildrenList.push_back(NewChild);
}

void Node::Update(class MainEngine* Engine, float Seconds, float DeltaSeconds)
{
    for (std::shared_ptr<Node> ChildNode : ChildrenList) {
        ChildNode->Update(Engine, Seconds, DeltaSeconds);
    }
}

bool Node::WasDirtyThisFrame() const
{
    return WasDirty;
}



