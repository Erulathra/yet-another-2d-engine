#ifndef SOLARSYSTEM_NODE_H
#define SOLARSYSTEM_NODE_H

#include <vector>
#include <memory>

#include "Transform.h"

class Node
{
private:
    std::shared_ptr<Transform> LocalTransformPtr;
    glm::mat4 WorldTransformMatrix;

    Node* Parent;
    std::vector<std::shared_ptr<Node>> ChildrenList;
    bool IsDirty;

    bool WasDirty;
public:
    explicit Node();

    void CalculateWorldTransform();
    void Draw();
    virtual void Update(class MainEngine* Engine, float Seconds, float DeltaSeconds);

    Transform* GetLocalTransform();

    void AddChild(std::shared_ptr<Node> NewChild);

    const glm::mat4* GetWorldTransformMatrix();

    [[nodiscard]] bool WasDirtyThisFrame() const;

protected:
    virtual void Draw(glm::mat4& ParentTransform, bool IsDirty);
    void CalculateWorldTransform(glm::mat4& ParentTransform, bool IsDirty);
};


#endif //SOLARSYSTEM_NODE_H
