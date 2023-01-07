#include "Nodes/ParallaxNode.h"
#include "MainEngine.h"
#include "Nodes/CameraNode.h"
#include "LoggingMacros.h"

void ParallaxNode::Start(struct MainEngine* engine) {
    Node::Start(engine);

    CameraNode* currentCamera = engine->GetCurrentCameraNode();

    lastCameraLocation = currentCamera->GetWorldPosition();
}

void ParallaxNode::Update(MainEngine* engine, float seconds, float deltaSeconds) {
    CameraNode* currentCamera = engine->GetCurrentCameraNode();
    glm::vec3 currentCameraLocation = currentCamera->GetWorldPosition();

    glm::vec3 cameraOffset = (lastCameraLocation - currentCameraLocation) * lagFactor;
    cameraOffset.z = 0;

    glm::vec3 newPosition = GetLocalTransform()->GetPosition() - cameraOffset;
    GetLocalTransform()->SetPosition(newPosition);

    lastCameraLocation = currentCameraLocation;
    Node::Update(engine, seconds, deltaSeconds);
}

void ParallaxNode::Draw(glm::mat4& parentTransform, bool isDirty) {
    Node::Draw(parentTransform, isDirty);
}

float ParallaxNode::GetLagFactor() const {
    return lagFactor;
}

void ParallaxNode::SetLagFactor(float lagFactor) {
    ParallaxNode::lagFactor = lagFactor;
}

ParallaxNode::ParallaxNode(float lagFactor)
: lagFactor(lagFactor), lastCameraLocation(0.f) {

}
