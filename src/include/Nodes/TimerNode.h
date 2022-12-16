#pragma once

#include "eventpp/callbacklist.h"
#include "Node.h"

class TimerNode : public Node {
    bool isOneShoot;
    bool isPaused;
    float timeLeft;
    float waitTime;

    TimerNode(Node* obj);

public:
    eventpp::CallbackList<void()> onTimeout;

    TimerNode(float waitTime);

    void Update(struct MainEngine* engine, float seconds, float deltaSeconds) override;
    std::shared_ptr<Node> Clone() const override;

    void StartTimer();

    void SetIsOneShoot(bool isOneShoot);
    void SetIsPaused(bool isPaused);
    void SetWaitTime(float waitTime);

    bool IsOneShoot() const;
    bool IsPaused() const;
    float GetTimeLeft() const;
    float GetWaitTime() const;
};
