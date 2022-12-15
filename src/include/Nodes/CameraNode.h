#pragma once


#include "Node.h"

class CameraNode : public Node {
private:
    std::unique_ptr<class Camera> camera;
    class MainEngine* engine;

    CameraNode();
    CameraNode(Node* node);
public:
    CameraNode(MainEngine* engine);
    virtual ~CameraNode();

    void Update(struct MainEngine* engine, float seconds, float deltaSeconds) override;
    void MakeCurrent();

    [[nodiscard]] std::shared_ptr<Node> Clone() const override;

    [[nodiscard]] float GetScale() const;
    void SetScale(float scale);
};
