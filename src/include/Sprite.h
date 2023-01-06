#pragma once

#include "VAOWrapper.h"

class Sprite {
private:
    glm::vec<2, int> TileMapPosition;
public:
    explicit Sprite(const glm::vec<2, int> &tileMapPosition);

    [[nodiscard]] const glm::vec<2, int> &GetTileMapPosition() const;

    friend class SpriteArrayNode;
};
