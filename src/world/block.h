#pragma once

#include <stdint.h>
#include <memory>
#include <glm/glm.hpp>

namespace yc::world {

enum class BlockType {
    AIR = 0,
    DIRT,
    GRASS,
    GRASS_BLOCK,
    SNOW,
    LEAF,
    SAND,
    STONE,
    WOOD,
    GLASS,
    WATER,
    RED_FLOWER,
    YELLOW_FLOWER,
    BLUE_FLOWER,
    NONE,
};

enum class BlockFaceDirection {
    UP = 0,
    DOWN,
    SOUTH,
    NORTH,
    EAST,
    WEST,
    NONE,
};

class BlockData {
public:

    static const BlockData Default;

    BlockData();
    BlockData(BlockType blockType);
    BlockData(BlockType blockType, BlockFaceDirection faceDirection);
    BlockType getType() const;
    BlockFaceDirection getFaceDirection() const;
    void setType(BlockType blockType);
    void setFaceDirection(BlockFaceDirection faceDirection);

    bool isOpaque();
    bool isTransparent();
    bool isFlora();

private:
    // |  bit  |    name    |
    // |  0-4  | block-type |
    // |  5-7  | block-dir  |
    uint8_t data;
};

class Chunk;

class Block {

public: 

    void reload();

private:

    std::shared_ptr<Chunk> chunk;

    glm::ivec3 worldCoordinate;
    glm::ivec3 chunkCoordinate;
    
    BlockType blockType;
    BlockFaceDirection faceDirection;
};

std::string GetBlockName(BlockType blockType);

}