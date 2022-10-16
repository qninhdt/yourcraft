#include "world/block.h"

namespace yc::world {

const BlockData BlockData::Default = { BlockType::AIR, BlockFaceDirection::NONE };

BlockData::BlockData() {
    // default block type == AIR, faceDirection == UP
    this->data = 0;
}

BlockData::BlockData(BlockType blockType): BlockData(blockType, BlockFaceDirection::NONE) {
}

BlockData::BlockData(BlockType blockType, BlockFaceDirection faceDirection) {
    this->data = 0;
    setType(blockType);
    setFaceDirection(faceDirection);
}

BlockType BlockData::getType() const {
    return static_cast<BlockType>(this->data & 0b11111);
}

BlockFaceDirection BlockData::getFaceDirection() const {
    return static_cast<BlockFaceDirection>((this->data >> 5));
}

void BlockData::setType(BlockType blockType) {
    uint32_t newTypeId = static_cast<uint32_t>(blockType);
    uint32_t oldTypeId = this->data & 0b11111;

    this->data -= oldTypeId;
    this->data += newTypeId;
}

void BlockData::setFaceDirection(BlockFaceDirection blockFaceDirection) {
    uint32_t newFaceDirectionId = static_cast<uint32_t>(blockFaceDirection);
    uint32_t oldFaceDirectionId = (this->data >> 5);

    this->data -= oldFaceDirectionId << 5;
    this->data += newFaceDirectionId << 5;
}

}