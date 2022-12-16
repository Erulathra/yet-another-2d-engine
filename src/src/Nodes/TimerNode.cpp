#include "Nodes/TimerNode.h"

bool TimerNode::IsOneShoot() const {
    return isOneShoot;
}

bool TimerNode::IsPaused() const {
    return isPaused;
}

float TimerNode::GetTimeLeft() const {
    return timeLeft;
}

float TimerNode::GetWaitTime() const {
    return waitTime;
}

void TimerNode::SetIsOneShoot(bool isOneShoot) {
    TimerNode::isOneShoot = isOneShoot;
}

void TimerNode::SetIsPaused(bool isPaused) {
    TimerNode::isPaused = isPaused;
}

void TimerNode::SetWaitTime(float waitTime) {
    TimerNode::waitTime = waitTime;
    if (isPaused) {
        timeLeft = waitTime;
    }
}

TimerNode::TimerNode(float waitTime)
        : isOneShoot(true), isPaused(true), waitTime(waitTime), timeLeft(waitTime) {
}

TimerNode::TimerNode(Node* obj)
        : isOneShoot(true), isPaused(true), waitTime(0), timeLeft(0) {

}

void TimerNode::Update(struct MainEngine* engine, float seconds, float deltaSeconds) {
    Node::Update(engine, seconds, deltaSeconds);

    if (isPaused)
        return;

    timeLeft -= deltaSeconds;

    if (timeLeft >= 0.f)
        return;

    onTimeout();

    if (isOneShoot) {
        isPaused = true;
        return;
    }

    timeLeft = waitTime;
}

std::shared_ptr<Node> TimerNode::Clone() const {
    std::shared_ptr<TimerNode> result(new TimerNode(Node::Clone().get()));
    result->isOneShoot = isOneShoot;
    result->isPaused = isPaused;
    result->timeLeft = timeLeft;
    result->waitTime = waitTime;
    return result;
}

void TimerNode::StartTimer() {
    isPaused = false;
}

