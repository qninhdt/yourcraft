#pragma once

#include <array>
#include <glm/glm.hpp>
#include "world/block.h"

namespace yc::graphic {

class BlockVertex {

public:

    static const std::array<std::array<BlockVertex, 4>, 6> Vertices;

    static const std::array<BlockVertex, 4>& GetVerticesFromDirection(const glm::ivec3& direction);

    BlockVertex();
    BlockVertex(uint32_t data);

    BlockVertex(const glm::uvec3& coordinate, const glm::uvec2& uv);

    void setBlockType(yc::world::BlockType blockType, const glm::ivec3& faceDirection);
    void moveCoordinate(int32_t dx, int32_t dy, int32_t dz);
    void setCoordinate(uint32_t x, uint32_t y, uint32_t z);
    void setTexure(uint32_t x, uint32_t y);
    void setUV(uint32_t x, uint32_t y);

    glm::uvec3 getCoordinate();

    uint32_t getData() const;

private:

    // |  bit  |  name  | 
    // |-------|--------|
    // |  0-4  |   x    | 0->31
    // |  5-9  |   z    | 0->31
    // | 10-18 |   y    | 0->511
    // | 19-19 |  x-uv  | 
    // | 20-20 |  y-uv  | 
    // | 21-24 | tex-x  |
    // | 25-28 | tex-y  | 
    uint32_t data;
};

}