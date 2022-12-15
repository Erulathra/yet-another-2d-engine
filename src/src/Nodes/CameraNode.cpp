#include "Nodes/CameraNode.h"
#include "Camera.h"
#include "MainEngine.h"

CameraNode::CameraNode(MainEngine* engine)
        : camera(std::make_unique<Camera>()), engine(engine) {

}

void CameraNode::Update(MainEngine* engine, float seconds, float deltaSeconds) {
    Node::Update(engine, seconds, deltaSeconds);

    if (engine->currentCameraNode != this) {
        return;
    }

    glm::vec<2, int> currentResolution{};
    glfwGetFramebufferSize(engine->window, &currentResolution.x, &currentResolution.y);

    camera->UpdateProjection(currentResolution);
    glm::vec3 targetPosition = GetWorldPosition();
    targetPosition.z = 25;
    camera->SetPosition(targetPosition);
}

std::shared_ptr<Node> CameraNode::Clone() const {
    std::shared_ptr<CameraNode> result(new CameraNode(Node::Clone().get()));
    result->camera = std::make_unique<Camera>(*camera);
    return Node::Clone();
}

CameraNode::CameraNode(Node* node)
        : Node(*node), camera(std::make_unique<Camera>()) {
}

void CameraNode::MakeCurrent() {
    engine->currentCameraNode = this;
}

CameraNode::CameraNode()
: camera(nullptr), engine(nullptr) {

}

CameraNode::~CameraNode() {

}

float CameraNode::GetScale() const {
    return camera->GetScale();
}

void CameraNode::SetScale(float scale) {
    camera->SetScale(scale);
}
