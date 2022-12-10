#pragma once

#include <vector>
#include <memory>

class CollisionShape {
public:
    virtual std::shared_ptr<CollisionShape>Clone() = 0;
};
