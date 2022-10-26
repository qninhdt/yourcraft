#include "graphic/block_vertex.h"
#include <iostream>

namespace yc::graphic {

const std::array<std::array<BlockVertex, 4>, 6> BlockVertex::Vertices = {{
  
    // front
    {{
        { { 1, 0, 1 }, { 1, 0 } }, 
        { { 1, 1, 1 }, { 1, 1 } }, 
        { { 0, 1, 1 }, { 0, 1 } }, 
        { { 0, 0, 1 }, { 0, 0 } }, 
    }},

    // back
    {{
        { { 1, 0, 0 }, { 0, 0 } }, 
        { { 1, 1, 0 }, { 0, 1 } }, 
        { { 0, 1, 0 }, { 1, 1 } }, 
        { { 0, 0, 0 }, { 1, 0 } }, 
    }},

    // right
    {{
        { { 1, 0, 0 }, { 1, 0 } }, 
        { { 1, 1, 0 }, { 1, 1 } }, 
        { { 1, 1, 1 }, { 0, 1 } }, 
        { { 1, 0, 1 }, { 0, 0 } },
    }},

    // left
    {{
        { { 0, 0, 0 }, { 0, 0 } }, 
        { { 0, 1, 0 }, { 0, 1 } }, 
        { { 0, 1, 1 }, { 1, 1 } }, 
        { { 0, 0, 1 }, { 1, 0 } },
    }},

    // top
    {{
        { { 1, 1, 0 }, { 1, 1 } }, 
        { { 1, 1, 1 }, { 1, 0 } }, 
        { { 0, 1, 1 }, { 0, 0 } }, 
        { { 0, 1, 0 }, { 0, 1 } }, 
    }},

    // bottom
    {{
        { { 1, 0, 1 }, { 0, 0 } }, 
        { { 1, 0, 0 }, { 0, 1 } }, 
        { { 0, 0, 0 }, { 1, 1 } }, 
        { { 0, 0, 1 }, { 1, 0 } },
    }},
}};

const std::array<BlockVertex, 4>& BlockVertex::GetVerticesFromDirection(const glm::ivec3& direction) {
    assert(abs(direction.x) + abs(direction.y) + abs(direction.z) == 1);

    if (direction.x == +1) { // right
        return Vertices[2];
    } else if (direction.x == -1) { // left
        return Vertices[3];
    } else if (direction.y == +1) { // top
        return Vertices[4];
    } else if (direction.y == -1) { // bottom
        return Vertices[5];
    } else if (direction.z == +1) { // front
        return Vertices[0];
    } else if (direction.z == -1) { // back
        return Vertices[1];
    }

    assert(false);
    return Vertices[0];
}

BlockVertex::BlockVertex() {
    this->data = 0;
}

BlockVertex::BlockVertex(uint32_t data) {
    this->data = data;
}

BlockVertex::BlockVertex(const glm::uvec3& coordinate, const glm::uvec2& uv) {
    setCoordinate(coordinate.x, coordinate.y, coordinate.z);
    setUV(uv.x, uv.y);
}

uint32_t BlockVertex::getData() const {
    return this->data;
}

void BlockVertex::setBlockType(yc::world::BlockType blockType, const glm::ivec3& faceDirection) {
    switch (blockType) {
        case yc::world::BlockType::GRASS_BLOCK:
            if (faceDirection.y == 1) {
                setTexure(0, 0);
            } else if (faceDirection.y == -1) {
                setTexure(1, 0);
            } else {
                setTexure(2, 0);
            }
            break;
        case yc::world::BlockType::DIRT:
            if (faceDirection.y == 1) {
                setTexure(3, 0);
            } else if (faceDirection.y == -1) {
                setTexure(4, 0);
            } else {
                setTexure(5, 0);
            }
            break;
        case yc::world::BlockType::STONE:
            if (faceDirection.y == 1 || faceDirection.y == -1) {
                setTexure(6, 0);
            } else {
                setTexure(7, 0);
            }
            break;
        case yc::world::BlockType::LEAF:
            setTexure(13, 0);
            break;
        case yc::world::BlockType::WOOD:
            if (faceDirection.y == 1 || faceDirection.y == -1) {
                setTexure(12, 0);
            } else {
                setTexure(11, 0);
            }
            break;
        case yc::world::BlockType::SNOW:
            setTexure(10, 0);
            break;
        case yc::world::BlockType::GLASS:
            setTexure(8, 0);
            break;
        case yc::world::BlockType::SAND:
            setTexure(9, 0);
            break;
        case yc::world::BlockType::WATER:
            if (faceDirection.y == 1) {
                setTexure(0, 15);
            } else if (faceDirection.y == -1) {
                setTexure(1, 15);
            } else {
                setTexure(2, 15);
            }
        default:
            break;
    }
}

glm::uvec3 BlockVertex::getCoordinate() {
    // (N & 0x1F)  => get last 5 bits
    // (N & 0x1FF) => get last 9 bits
    return {
        (this->data >> 0)  & 0x1F,
        (this->data >> 10) & 0x1FF,
        (this->data >> 5)  & 0x1F,
    };
}

void BlockVertex::setCoordinate(uint32_t x, uint32_t y, uint32_t z) {
    assert(x <= 16);
    assert(y <= 256);
    assert(z <= 16);

    this->data += x << 0;
    this->data += z << 5;
    this->data += y << 10;
}

void BlockVertex::moveCoordinate(int32_t dx, int32_t dy, int32_t dz) {
    glm::uvec3 pos = getCoordinate();

    this->data += dx << 0;
    this->data += dz << 5;
    this->data += dy << 10;
}

void BlockVertex::setTexure(uint32_t x, uint32_t y) {
    assert(x < 16);
    assert(y < 16);

    this->data += (x | (y << 4)) << 21;
}

void BlockVertex::setUV(uint32_t x, uint32_t y) {
    assert(x==0 || x==1);
    assert(y==0 || y==1);

    this->data |= x << 19;
    this->data |= y << 20;
}

}