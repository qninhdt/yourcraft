#include "world/block.h"
#include <string>

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

bool BlockData::isOpaque() {
    return !BlockData::isTransparent() && !isFlora();
}

bool BlockData::isTransparent() {
    BlockType blockType = getType();
    return blockType == BlockType::AIR
        || blockType == BlockType::GLASS
        || blockType == BlockType::LEAF
        || blockType == BlockType::WATER;
}

bool BlockData::isFlora() {
    BlockType blockType = getType();
    return blockType == BlockType::GRASS
        || blockType == BlockType::RED_FLOWER
        || blockType == BlockType::YELLOW_FLOWER
        || blockType == BlockType::BLUE_FLOWER;
}

std::string GetBlockName(BlockType blockType) {
    switch (blockType) {
        case BlockType::GRASS_BLOCK:
            return "Grass Block";
        case BlockType::DIRT:
            return "Dirt Block";
        case BlockType::STONE:
            return "Stone Block";
        case BlockType::LEAF:
            return "Leaf";
        case BlockType::WOOD:
            return "Wood Block";
        case BlockType::SNOW:
            return "Snow Block";
        case BlockType::GLASS:
            return "Glass Block";
        case BlockType::SAND:
            return "Sand Block";
        case BlockType::WATER:
            return "Water";
        case BlockType::RED_FLOWER:
            return "Red Flower";
        case BlockType::BLUE_FLOWER:
            return "Blue Flower";
        case BlockType::YELLOW_FLOWER:
            return "Yellow Flower";
        case BlockType::GRASS:
            return "Grass";
        default:
            return "None";
    }
}

}