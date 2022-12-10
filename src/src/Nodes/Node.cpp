#include "Nodes/Node.h"
#include "LoggingMacros.h"

Node::Node()
: localTransform(std::make_shared<Transform>()), worldTransformMatrix(1.f)
{

}

Transform* Node::GetLocalTransform()
{
    return localTransform.get();
}

const glm::mat4* Node::GetWorldTransformMatrix()
{
    return &worldTransformMatrix;
}

void Node::Draw()
{
    glm::mat4 WorldMatrix = *GetWorldTransformMatrix();
    Draw(WorldMatrix, localTransform->isDirty);
}

void Node::CalculateWorldTransform()
{
    glm::mat4 LocalTransformMatrix = localTransform->GetMatrix();
    CalculateWorldTransform(LocalTransformMatrix, localTransform->isDirty);
}

void Node::Draw(glm::mat4& parentTransform, bool isDirty)
{
    for (const std::shared_ptr<Node>& Child: childrenList)
    {
        Child->Draw(worldTransformMatrix, isDirty);
    }
}

void Node::CalculateWorldTransform(glm::mat4& parentTransform, bool isDirty)
{
    isDirty |= localTransform->isDirty;
    wasDirty = isDirty;
    if (isDirty)
    {
        worldTransformMatrix = parentTransform * localTransform->GetMatrix();
        localTransform->isDirty = false;
    }


    for (const std::shared_ptr<Node>& child: childrenList)
    {
        child->CalculateWorldTransform(worldTransformMatrix, isDirty);
    }
}

void Node::AddChild(std::shared_ptr<Node> newChild)
{
    if (newChild.get() == this || newChild.get() == Parent)
        return;

    newChild->Parent = newChild.get();
    childrenList.push_back(newChild);
    newChild->CalculateWorldTransform(worldTransformMatrix, true);
}

void Node::Update(class MainEngine* engine, float seconds, float deltaSeconds)
{
    for (std::shared_ptr<Node> childNode : childrenList) {
        childNode->Update(engine, seconds, deltaSeconds);
    }
}

bool Node::WasDirtyThisFrame() const
{
    return wasDirty;
}

std::shared_ptr<Node> Node::Clone() const {
    auto result = std::make_shared<Node>();
    result->localTransform= std::make_shared<Transform>(*this->localTransform) ;
    result->wasDirty = true;

    for (const auto& node : childrenList) {
        result->AddChild(node->Clone());
    }

    return result;
}



