#ifndef SOLARSYSTEM_NODE_H
#define SOLARSYSTEM_NODE_H

#include <vector>
#include <memory>

#include "Transform.h"

class Node
{
private:
    std::shared_ptr<Transform> localTransform;
    glm::mat4 worldTransformMatrix;

    Node* Parent;
    std::vector<std::shared_ptr<Node>> childrenList;
    bool isDirty;

    bool wasDirty;
public:
    explicit Node();

    void CalculateWorldTransform();
    void Draw();
    virtual void Update(class MainEngine* engine, float seconds, float deltaSeconds);

    Transform* GetLocalTransform();

    void AddChild(std::shared_ptr<Node> newChild);

    const glm::mat4* GetWorldTransformMatrix();

    [[nodiscard]] bool WasDirtyThisFrame() const;

    virtual std::shared_ptr<Node> Clone();

protected:
    virtual void Draw(glm::mat4& parentTransform, bool isDirty);
    void CalculateWorldTransform(glm::mat4& parentTransform, bool isDirty);
};


#endif //SOLARSYSTEM_NODE_H
