#pragma once


#include "Node.h"

class ParallaxNode : public Node {
private:
    float lagFactor;
    glm::vec3 lastCameraLocation;

public:
    ParallaxNode(float lagFactor);

    void Start(struct MainEngine* engine) override;

    void Update(struct MainEngine* engine, float seconds, float deltaSeconds) override;


    [[nodiscard]] float GetLagFactor() const;

    void SetLagFactor(float lagFactor);

protected:
    void Draw(glm::mat4& parentTransform, bool isDirty) override;

};
