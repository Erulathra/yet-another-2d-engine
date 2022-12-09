#include "Sprite.h"

Sprite::Sprite(const glm::vec<2, int> &tileMapPosition) : TileMapPosition(tileMapPosition) {}

const glm::vec<2, int> &Sprite::GetTileMapPosition() const {
    return TileMapPosition;
}
